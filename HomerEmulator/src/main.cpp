/*
 * main.cpp
 *
 *  Created on: 07/nov/2016
 *      Author: daniele
 *
 * FIXME: File Header refactoring (import new template )
 * FIXME: Comment and other style decoration
 * FIXME: Sensor and menu refactoring
 * FIXME: Decorator pattern for Menu ?
 * FIXME: Factory pattern for creation ?
 * FiXME: getter with const getXxxx const
 *
 *
 */

#include <Menu.hpp>
#include <iostream>
#include <KeyEmulator.hpp>
#include <KeyPanel.hpp>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "Scheduler.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/tchar.h>
#include <DisplayVisitor.hpp>
#include <HomerEmulator.hpp>
#include <MoveVisitor.hpp>
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <HwEmulated.hpp>
#include <Sensor.hpp>
#include <WinstarEmulator.hpp>

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
  PressureSensor *pSens;

  // Emulated stuff
  BoardEmulated *acquaA5;
  WinstarEmulator *display;
  HomerEmulator *emulator;

  // Create Universe
  keyPanel = new KeyPanel();
  scheduler = new Scheduler();
  acquaA5 = new BoardEmulated();
  display = new WinstarEmulator(*keyPanel, *scheduler, *acquaA5);
  emulator = new HomerEmulator(display);
  shared_ptr < MenuActionVisitor > dw(new DisplayVisitor(*display));
  Registration preg, treg;
  acquaA5->getEmulatedSysFs().reg_read(
      treg, string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/temp0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        static int vt = 200;
        vt+=5;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });

  acquaA5->getEmulatedSysFs().reg_read(
      preg, string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/pressure0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        static int vp = 99614-50;
        vp+=50;
        sprintf((char *)buffer,"%d",vp);
        *ret = (int)strlen((char *)buffer);
      });

  bmp085Device = new Bmp085Device(*acquaA5);
  tSens = new TemperatureSensor(*bmp085Device, string("Temperature"));
  pSens = new PressureSensor(*bmp085Device, string("Pressure"));
  pSens->setAltituteCalibration(354.0);

  // life spark ignition
  emulator->start();
  display->reset();
  keyPanel->set_event_filename(emulator->getKeyEventFilename().c_str());
  keyPanel->start();

  tSens->update();
  pSens->update();

  // Populating universe
  menu = new HomerMenu(*keyPanel, *scheduler, *tSens, *pSens);
  menu->addActionVisitor(dw);
  display->set_backlight(true);

  LOG4CPLUS_INFO(logemu, "HomerEmulator ready ...");

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
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);

  // give back power
  return 0;
}
