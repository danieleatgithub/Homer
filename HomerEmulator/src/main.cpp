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
#include <CsvSensorDecorator.hpp>
#include <SensorManager.hpp>
#include <EmulatedHw.hpp>
#include <EmulatedDevices.hpp>
#include <HomerEmulator.hpp>
#include <WinstarEmulator.hpp>
#include <KeyEmulator.hpp>
#include <MenuDisplayVisitor.hpp>
#include <MenuMoveVisitor.hpp>
#include <Bmp085Device.hpp>
#include <Ina219Device.hpp>
#include <IPAddressSensor.hpp>
#include <CurrentSensor.hpp>
#include <PowerSensor.hpp>
#include <VoltageSensor.hpp>

using namespace std;
using namespace homerio;
using namespace commodities;
using namespace log4cplus;
using namespace homeremulator;

struct termios t;

int main(int argc, char** argv) {

  // Poperties load and init

  initialize();
  PropertyConfigurator config(
      "/wks/workspace/Homer/HomerEmulator/resources/homerd.properties");
  config.configure();

  Logger logger = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_INFO(logger, "HomerEmulator starting ...");

  // Common stuff
  Scheduler *scheduler;
  KeyPanel *keyPanel;
  HomerMenu *menu;
  Bmp085Device *bmp085Device;
  Ina219Device *ina219Device;
  TemperatureSensor *tSens;
  BarometricSensor *pSens;
  IPAddressSensor *ipSens;
  CurrentSensor *aSens;
  PowerSensor *wSens;
  VoltageSensor *vSens;
  VoltageSensor *rsSens;

  SensorManager *sensorManager;

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
  sensorManager = new SensorManager(*scheduler, std::chrono::seconds(10));
  display = new WinstarEmulator(*keyPanel, *scheduler, *acquaA5);

  emulator = new HomerEmulator(display);

  shared_ptr < MenuActionVisitor > dw(new MenuDisplayVisitor(*display));

  menu = new HomerMenu(*keyPanel, *scheduler);

  // Create sensors Orion Nebula
  bmp085Device = new Bmp085Device(*acquaA5);
  ina219Device = new Ina219Device(*acquaA5);

  tSens = new TemperatureSensor(bmp085Device->getThermometer(),
                                string("Temperature"));
  sensorManager->add(*tSens);
  menu->addSensor(*tSens);

  pSens = new BarometricSensor(bmp085Device->getBarometer(),
                               string("Pressure"));
  pSens->setAltituteCalibration(354.0);
  sensorManager->add(*pSens);
  menu->addSensor(*pSens);

  aSens = new CurrentSensor(ina219Device->getCurrent(), string("Current"));
  sensorManager->add(*aSens);
  menu->addSensor(*aSens);

  wSens = new PowerSensor(ina219Device->getPower(), string("Power"));
  sensorManager->add(*wSens);
  menu->addSensor(*wSens);

  rsSens = new VoltageSensor(ina219Device->getRsensVolts(),
                             string("Shunt Voltage"));
  sensorManager->add(*rsSens);
  menu->addSensor(*rsSens);

  vSens = new VoltageSensor(ina219Device->getVoltage(), string("Voltage"));
  sensorManager->add(*vSens);
  menu->addSensor(*vSens);

  ipSens = new IPAddressSensor(string("eth0"), string("IpAddress:"));
  sensorManager->add(*ipSens);
  menu->addSensor(*ipSens);

  // life spark ignition
  emulatedDev->start();
  emulator->start();
  sensorManager->start();
  display->reset();
  keyPanel->setEventFilename(emulator->getKeyEventFilename().c_str());
  keyPanel->start();
  menu->start();
  menu->addActionVisitor(dw);
  display->set_backlight(true);

  LOG4CPLUS_INFO(logger, "HomerEmulator ready ...");

  // waiting for armageddon
  emulator->mainLoop();

  // end of life
  keyPanel->stop();
  menu->stop();
  sensorManager->stop();
  emulator->stop();
  emulatedDev->stop();

  // one second silence for our threads
  sleep(1);

  // keep clear empty space
  delete (display);
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);
  delete (tSens);
  delete (aSens);
  delete (wSens);
  delete (rsSens);
  delete (vSens);
  delete (menu);
  delete (sensorManager);
  delete (emulatedDev);
  delete (emulator);

  // ready for resurrection
  LOG4CPLUS_INFO(logger, "HomerEmulator exit");
  return 0;
}
