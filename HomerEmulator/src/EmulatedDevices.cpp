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
#include <EmulatedDevices.hpp>

namespace homeremulator {

EmulatedDevices::EmulatedDevices(BoardEmulated& _board)
    : board(_board) {
}

EmulatedDevices::~EmulatedDevices() {
}

void EmulatedDevices::temperature_register() {
  board.getEmulatedSysFs().reg_read(
      temperature_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/temp0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vt = 200;
        vt+=5;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::barometric_register() {
  board.getEmulatedSysFs().reg_read(
      barometric_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/pressure0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vp = 99614-50;
        vp+=50;
        sprintf((char *)buffer,"%d",vp);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::current_register() {
  board.getEmulatedSysFs().reg_read(
      current_reg,
      string(
          "/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/curr1_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 123467;
        va+=50;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}

void EmulatedDevices::start() {
  temperature_register();
  barometric_register();
  current_register();
}
void EmulatedDevices::stop() {
  // Observers will be unregisterd in destructor
}

}
/* namespace homeremulator */
