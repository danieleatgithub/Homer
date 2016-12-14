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
#include <Menu.hpp>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/tchar.h>
#include <SimpleMenuElement.hpp>
#include <KeyPanel.hpp>
#include <EmulatedDevices.hpp>
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <Sensor.hpp>
#include <EmulatedHw.hpp>
#include <HomerEmulator.hpp>
#include <WinstarEmulator.hpp>
#include <KeyEmulator.hpp>
#include <MenuDisplayVisitor.hpp>
#include <MenuMoveVisitor.hpp>
#include <Scheduler.hpp>

namespace homeremulator {

EmulatedDevices::EmulatedDevices(BoardEmulated& _board)
    : board(_board) {
}

EmulatedDevices::~EmulatedDevices() {

}

void EmulatedDevices::temp_panel_register() {
  board.getEmulatedSysFs().reg_read(
      temp_panel_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/temp0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        static int vt = 200;
        vt+=5;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}

void EmulatedDevices::bar_panel_register() {
  board.getEmulatedSysFs().reg_read(
      bar_panel_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/pressure0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        static int vp = 99614-50;
        vp+=50;
        sprintf((char *)buffer,"%d",vp);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::register_all_emulator() {
  temp_panel_register();
  bar_panel_register();
}
}
/* namespace homeremulator */
