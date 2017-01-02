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
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <Scheduler.hpp>
#include <sensor/CsvSensorDecorator.hpp>

#include <EmulatedHw.hpp>
#include <EmulatedDevices.hpp>
#include <HomerEmulator.hpp>
#include <WinstarEmulator.hpp>
#include <KeyEmulator.hpp>
#include <MenuDisplayVisitor.hpp>
#include <MenuMoveVisitor.hpp>
#include <Bmp085Device.hpp>

using namespace std;
using namespace homerio;
using namespace shd;
using namespace log4cplus;
using namespace homeremulator;

struct termios t;

int main(int argc, char** argv) {

  // Poperties load and init
  initialize();
  PropertyConfigurator config(
      "/wks/workspace/Homer/HomerEmulator/resources/homerd.properties");
  config.configure();

  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_INFO(logemu, "HomerEmulator starting ...");

  // Real stuff
  Scheduler *scheduler;
  KeyPanel *keyPanel;
  HomerMenu *menu;
  Bmp085Device *bmp085Device;
  TemperatureSensor *tSens;
  BarometricSensor *pSens;
  CsvSensorDecorator *tCsv;
  CsvSensorDecorator *pCsv;

  // Emulated stuff
  BoardEmulated *acquaA5;
  WinstarEmulator *display;
  HomerEmulator *emulator;
  EmulatedDevices *emulatedDev;

  // Create Universe
  keyPanel = new KeyPanel();
  scheduler = new Scheduler();
  acquaA5 = new BoardEmulated();
  emulatedDev = new EmulatedDevices(*acquaA5);

  display = new WinstarEmulator(*keyPanel, *scheduler, *acquaA5);
  emulator = new HomerEmulator(display);
  shared_ptr < MenuActionVisitor > dw(new MenuDisplayVisitor(*display));

  bmp085Device = new Bmp085Device(*acquaA5);
  tSens = new TemperatureSensor(*bmp085Device, string("Temperature"));
  pSens = new BarometricSensor(*bmp085Device, string("Pressure"));
  pSens->setAltituteCalibration(354.0);

  tCsv = new CsvSensorDecorator(*tSens);
  pCsv = new CsvSensorDecorator(*pSens);

  // life spark ignition
  emulatedDev->start();
  emulator->start();
  display->reset();
  keyPanel->setEventFilename(emulator->getKeyEventFilename().c_str());
  keyPanel->start();

  // Populating universe
  menu = new HomerMenu(*keyPanel, *scheduler, *tSens, *pSens);
  menu->addActionVisitor(dw);
  display->set_backlight(true);

  LOG4CPLUS_INFO(logemu, "HomerEmulator ready ...");
  tSens->update();
  pSens->update();

  cout << tCsv->getString();
  cout << pCsv->getString() << endl;

  // wait for armageddon
  emulator->mainLoop();

  // end of life
  keyPanel->stop();
  emulator->stop();

  // one second silence for our threads
  sleep(1);

  // keep clear empty space
  delete (emulator);
  delete (menu);
  delete (display);
  delete (emulatedDev);
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);

  // give back power
  return 0;
}
