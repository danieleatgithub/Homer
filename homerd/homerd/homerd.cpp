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

/*
 * homerd.cpp
 *
 *  FIXME: set altitude from properties
 *  FIXME: check coredump generation
 *  TODO: const fun const revision
 *  TODO: change altitude from key panel
 *  TODO: move to cmake or check valid target in autotools
 *
 *
 */

#include <Display.h>
#include <fcntl.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/tchar.h>
#include <optionparser.h>
#include <stdio.h>
#include <sys/stat.h>
#include <Sysinfo.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include "homerd.h"
#include "KeyPanel.hpp"
#include "Observer.h"
#include "Scheduler.hpp"
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <Winstar.h>
#include <HwAcquaA5.hpp>
#include <CsvSensorDecorator.hpp>
#include <SensorManager.hpp>
#include <Bmp085Device.hpp>
#include <Ina219Device.hpp>
#include <IPAddressSensor.hpp>
#include <CurrentSensor.hpp>
#include <PowerSensor.hpp>
#include <VoltageSensor.hpp>

using option::Option;
using option::Descriptor;
using option::Parser;
using option::Stats;
using option::ArgStatus;
using namespace std;
using namespace log4cplus;
using namespace homerio;
using namespace obs;
using namespace commodities;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

#define HOMERD_DEF_PROPS  "/etc/default/homerd.properties"
#define HOMERD_DEF_OUT_REDIR "/data/logs/homerd_out.log"
#define HOMERD_DEF_ERR_REDIR "/data/logs/homerd_err.log"

enum optionIndex {
  UNKNOWN,
  HELP,
  DAEMON,
  RSTDERR,
  RSTDOUT,
  PROPS
};
// clang-format off
// *INDENT-OFF*
const option::Descriptor usage[] = { { UNKNOWN, 0, "", "", option::Arg::None,
    "USAGE: homerd [options]\n\nOptions:" }, { HELP, 0, "h", "help",
    option::Arg::None, "  --help  \tPrint usage and exit." }, { DAEMON, 0, "d",
    "daemon", option::Arg::None, "  --daemon  \tStart as daemon." }, { RSTDERR,
    0, "e", "err", option::Arg::Optional,
    "  --err, -e  \tredirect stderr to file." }, { RSTDOUT, 0, "o", "out",
    option::Arg::Optional, "  --out, -o  \tredirect stdout to file." },
    { PROPS, 0, "c", "conf", option::Arg::Optional,
        "  --conf, -c  \tconfiguration file." }, { 0, 0, 0, 0, 0, 0 } };
// *INDENT-ON*
// clang-format on

const char *std_err = 0;
const char *std_out = 0;
const char *props_file = HOMERD_DEF_PROPS;
bool daemon_mode = false;

int main(int argc, char *argv[]) {

  pid_t pid, sid;
  argc -= (argc > 0);
  argv += (argc > 0);  // skip program name argv[0] if present
  option::Stats stats(usage, argc, argv);

#ifdef __GNUC__
  // GCC supports C99 VLAs for C++ with proper constructor calls.
  option::Option options[stats.options_max], buffer[stats.buffer_max];
#else
  // use calloc() to allocate 0-initialized memory. It's not the same
  // as properly constructed elements, but good enough. Obviously in an
  // ordinary C++ program you'd use new[], but this file demonstrates that
  // TLMC++OP can be used without any dependency on the C++ standard library.
  option::Option *options = (option::Option *) calloc(stats.options_max,
                                                      sizeof(option::Option));
  option::Option *buffer = (option::Option *) calloc(stats.buffer_max,
                                                     sizeof(option::Option));
#endif

  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error())
    return 1;

  if (options[HELP]) {
    int columns = getenv("COLUMNS") ? atoi(getenv("COLUMNS")) : 80;
    option::printUsage(fwrite, stdout, usage, columns);
    return 0;
  }
  for (int i = 0; i < parse.optionsCount(); ++i) {
    option::Option &opt = buffer[i];
    switch (opt.index()) {
      case HELP:
        // not possible, because handled further above and exits the program
      case RSTDERR:
        if (opt.arg)
          std_err = opt.arg;
        else
          std_err = HOMERD_DEF_ERR_REDIR;
        break;
      case RSTDOUT:
        if (opt.arg)
          std_out = opt.arg;
        else
          std_out = HOMERD_DEF_OUT_REDIR;
        break;
      case DAEMON:
        daemon_mode = true;
        break;
      case PROPS:
        if (opt.arg)
          props_file = opt.arg;
        break;
      case UNKNOWN:
        // not possible because Arg::Unknown returns ARG_ILLEGAL
        // which aborts the parse with an error
        break;
    }
  }

  if (daemon_mode) {
    pid = fork();
    if (pid < 0) {
      exit (EXIT_FAILURE);
    }
    // We got a good pid, Close the Parent Process
    if (pid > 0) {
      exit (EXIT_SUCCESS);
    }
    // Change File Mask
    umask(0);
    // Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) {
      exit (EXIT_FAILURE);
    }
    // Change Directory
    // If we cant find the directory we exit with failure.
    if ((chdir("/tmp")) < 0) {
      exit (EXIT_FAILURE);
    }
    // Close Standard File Descriptors
    close (STDIN_FILENO);
    close (STDOUT_FILENO);
    close (STDERR_FILENO);
    /* redirect fd's 0,1,2 to /dev/null or debug files*/
    open("/dev/null", O_RDWR);
    if (std_out)
      open(std_out, O_RDWR);
    else
      dup(0);
    if (std_err)
      open(std_err, O_RDWR);
    else
      dup(0);
  }

  string ip;
  bool run = true;
  Sysinfo sysinfo = Sysinfo::get_instance();

  initialize();
  PropertyConfigurator config(props_file);
  config.configure();

  Logger logger = Logger::getRoot();
  LOG4CPLUS_INFO(logger, "Homer starting ... ");

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
  TemperatureSensor *tSens;
  BarometricSensor *pSens;
  IPAddressSensor *ipSens;
  CurrentSensor *aSens;
  PowerSensor *wSens;
  VoltageSensor *vSens;
  VoltageSensor *rsSens;

  SensorManager *sensorManager;

  //  Real stuff
  BoardAcquaA5 *acquaA5;
  Display *display;

  // Create Universe
  keyPanel = new KeyPanel();
  scheduler = new Scheduler();
  acquaA5 = new BoardAcquaA5();
  sensorManager = new SensorManager(*scheduler);
  display = new Winstar(*keyPanel, *scheduler, *acquaA5);

  keyPanel->setEventFilename(KEY_EVENT_DEVICE);

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

  tSens = new TemperatureSensor(*bmp085Thermometer, string("Temperature"));
  sensorManager->add(*tSens);
  menu->addSensor(*tSens);

  pSens = new BarometricSensor(*bmp085Barometer, string("Pressure"));
  pSens->setAltituteCalibration(354.0);
  sensorManager->add(*pSens);
  menu->addSensor(*pSens);

  aSens = new CurrentSensor(*ina219Current, string("Current"));
  aSens->setScale(-3);
  sensorManager->add(*aSens);
  menu->addSensor(*aSens);

  wSens = new PowerSensor(*ina219Power, string("Power"));
  sensorManager->add(*wSens);
  menu->addSensor(*wSens);

  rsSens = new VoltageSensor(*ina219Rsens, string("Shunt Voltage"));
  rsSens->setScale(-3);
  sensorManager->add(*rsSens);
  menu->addSensor(*rsSens);

  vSens = new VoltageSensor(*ina219Voltage, string("Voltage"));
  sensorManager->add(*vSens);
  menu->addSensor(*vSens);

  ipSens = new IPAddressSensor(string("eth0"), string("IpAddress:"));
  sensorManager->add(*ipSens);
  menu->addSensor(*ipSens);

  sleep(1);

  // life spark ignition
  sensorManager->start();
  display->reset();
  menu->addActionVisitor(dw);
  menu->start();
  keyPanel->start();

  LOG4CPLUS_INFO(logger, "homerd ready");

  // waiting for armageddon
  while (true) {
    this_thread::sleep_for(std::chrono::seconds(10));
    // FIXME: condwait in sysinfo per exit
    if (keyPanel->getKeyCounter() > 999999999)
      break;
  }

  keyPanel->stop();
  menu->stop();
  sensorManager->stop();
  sleep(1);

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
  delete (display);
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);

  // ready for resurrection
  LOG4CPLUS_INFO(logger, "homerd exit");
  return 0;
}
