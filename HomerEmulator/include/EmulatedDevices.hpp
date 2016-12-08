/*
 * EmulatedDevice.hpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */
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
