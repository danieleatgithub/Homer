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
#include <Hs1101lfDevice.hpp>
#include <IPAddressSensor.hpp>
#include <CurrentSensor.hpp>
#include <PowerSensor.hpp>
#include <VoltageSensor.hpp>
#include <HumiditySensor.hpp>

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
  Bmp085Thermometer *bmp085Thermometer;
  Bmp085Barometer *bmp085Barometer;

  Ina219Device *ina219Device;
  Ina219Current *ina219Current;
  Ina219Voltage *ina219Voltage;
  Ina219Rsens *ina219Rsens;
  Ina219Power *ina219Power;

  Hs1101lfDevice *hs1101lfDevice;
  Hs1101lfHumidity *hs1101lfHumidity;

  TemperatureSensor *tSens;
  BarometricSensor *pSens;
  IPAddressSensor *ipSens;
  CurrentSensor *aSens;
  PowerSensor *wSens;
  VoltageSensor *vSens;
  VoltageSensor *rsSens;
  HumiditySensor *rhSens;

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
  bmp085Thermometer = new Bmp085Thermometer(*bmp085Device);
  bmp085Barometer = new Bmp085Barometer(*bmp085Device);

  ina219Device = new Ina219Device(*acquaA5);
  ina219Current = new Ina219Current(*ina219Device);
  ina219Voltage = new Ina219Voltage(*ina219Device);
  ina219Rsens = new Ina219Rsens(*ina219Device);
  ina219Power = new Ina219Power(*ina219Device);

  hs1101lfDevice = new Hs1101lfDevice(*acquaA5);
  hs1101lfHumidity = new Hs1101lfHumidity(*hs1101lfDevice, *bmp085Thermometer);

  tSens = new TemperatureSensor(*bmp085Thermometer, string("Temperature"));
  sensorManager->add(*tSens);
  menu->addSensor(*tSens);

  pSens = new BarometricSensor(*bmp085Barometer, string("Pressure"));
  pSens->setAltituteCalibration(LOCAL_ALTITUDE);
  sensorManager->add(*pSens);
  menu->addSensor(*pSens);

  aSens = new CurrentSensor(*ina219Current, string("Current"));
  aSens->setScale(-3);
  sensorManager->add(*aSens);
  menu->addSensor(*aSens);

  wSens = new PowerSensor(*ina219Power, string("Power"));
  wSens->setScale(-3);
  sensorManager->add(*wSens);
  menu->addSensor(*wSens);

  rsSens = new VoltageSensor(*ina219Rsens, string("Shunt Voltage"));
  rsSens->setScale(-3);
  sensorManager->add(*rsSens);
  menu->addSensor(*rsSens);

  vSens = new VoltageSensor(*ina219Voltage, string("Voltage"));
  sensorManager->add(*vSens);
  menu->addSensor(*vSens);

  rhSens = new HumiditySensor(*hs1101lfHumidity, string("Humidity"));
  sensorManager->add(*rhSens);
  menu->addSensor(*rhSens);

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
  delete (tSens);
  delete (aSens);
  delete (wSens);
  delete (rsSens);
  delete (vSens);
  delete (ina219Current);
  delete (ina219Voltage);
  delete (ina219Rsens);
  delete (ina219Power);
  delete (bmp085Device);
  delete (ina219Device);
  delete (sensorManager);
  delete (menu);
  delete (emulator);
  delete (display);
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);
  delete (emulatedDev);

  // ready for resurrection
  LOG4CPLUS_INFO(logger, "HomerEmulator exit");
  return 0;
}
