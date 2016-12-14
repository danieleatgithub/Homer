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
#include <KeyPanel.hpp>
#include <SimpleMenuElement.hpp>
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

#ifndef SRC_EMULATEDDEVICE_HPP_
#define SRC_EMULATEDDEVICE_HPP_

namespace homeremulator {

class EmulatedDevices {
 public:
  EmulatedDevices(BoardEmulated& _board);
  virtual ~EmulatedDevices();
  void register_all_emulator();

 private:
  BoardEmulated& board;
  Registration bar_panel_reg, temp_panel_reg;

  void temp_panel_register();
  void bar_panel_register();
};

}
/* namespace homeremulator */

#endif /* SRC_EMULATEDDEVICE_HPP_ */
