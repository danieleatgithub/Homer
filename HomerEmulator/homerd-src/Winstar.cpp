/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Winstar.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <GpioPin.h>

using namespace std;
using namespace log4cplus;

namespace homerio {

const unsigned int freqency_3_volts[8] = { 122, 131, 144, 161, 183, 221, 274,
    347 };
const unsigned int freqency_5_volts[8] = { 120, 133, 149, 167, 192, 227, 277,
    347 };

void Winstar::init() {
  this->entry_mode = 0;
  this->display_mode = 0;
  this->function_set = 0;
  this->ddram_addr = 0;
  // Normal instruction set (IS=0)
  this->cursor_display_shift = 0;
  // Extended instruction set (IS=1)
  this->bias_osc_frequency_adj = 0;
  this->icon_ram_address = 0;
  this->pow_icon_contrast = 0;
  this->follower = 0;
  this->contrast_set = 0;
  this->address = WINSTAR_I2C_ADD;

}

Winstar::Winstar(KeyPanel &kpnl, Scheduler &shd, Board& board)
    : Display(kpnl, shd, board.getI2c0(), board.getLcdReset(),
              board.getLcdBacklight()) {
  init();
}

Winstar::~Winstar() {
}

int Winstar::set_normal_mode() {
  function_set &= ~WSTAR_FUNCTION_EXTENDED;
  return (write_cmd(function_set));
}
int Winstar::set_extended_mode() {
  function_set |= WSTAR_FUNCTION_EXTENDED;
  return (write_cmd(function_set));
}

int Winstar::write_cmd(unsigned char data) {
  return (this->dpy_write(WSTAR_CMD, data));
}
int Winstar::write_data(unsigned char data) {
  return (this->dpy_write(WSTAR_DATA, data));
}
int Winstar::dpy_write(int type, uint8_t data) {
  unsigned char buffer[2];
  buffer[0] = (unsigned char) type;
  buffer[1] = data;
  Logger logdev = Logger::getInstance(LOGDEVICE);

  // TODO: Change to singleton and add a semaphore on i2cbus usage
  int fd = i2cBus.open(bus.c_str(), O_RDWR);
  if (fd < 0)
    return (fd);
  if (i2cBus.ioctl(fd, I2C_SLAVE, this->address) < 0) {
    LOG4CPLUS_ERROR(logdev, "ioctl error: " << strerror(errno) << "\n");
    i2cBus.close(fd);
    return -1;
  }
  LOG4CPLUS_TRACE(
      logdev,
      "i2cset -y 0 0x" << hex << (unsigned int) address << " 0x" << hex << type
          << " 0x" << hex << (unsigned int) data);
  if (i2cBus.write(fd, buffer, 2) != 2) {
    LOG4CPLUS_ERROR(logdev, "write error  " << strerror(errno));
    return -1;
  }
  i2cBus.close(fd);
  usleep (write_usleep);
  return (0);
}

}

