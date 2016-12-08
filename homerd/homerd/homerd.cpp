/*
 ============================================================================
 Name        : homerd.cpp
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Hello World in C++,

 FIXME: set altitude from properties
 FIXME: check coredump generation
 TODO: const fun const revision
 TODO: change altitude from key panel
 TODO: move to cmake or check valid target in autotools

 ============================================================================
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
#include <DisplayVisitor.hpp>
#include <MoveVisitor.hpp>
#include <HomerMenu.hpp>
#include <HwLayer.hpp>
#include <Winstar.h>
#include <HwAcquaA5.hpp>

using option::Option;
using option::Descriptor;
using option::Parser;
using option::Stats;
using option::ArgStatus;
using namespace std;
using namespace log4cplus;
using namespace homerio;
using namespace obs;
using namespace shd;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

#define HOMERD_DEF_PROPS  "/etc/default/homerd.properties"
#define HOMERD_DEF_OUT_REDIR "/var/log/homerd_out.log"
#define HOMERD_DEF_ERR_REDIR "/var/log/homerd_err.log"

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

  Display *display;
  Scheduler *scheduler;
  KeyPanel *keyPanel;
  HomerMenu *menu;
  BoardAcquaA5 *acquaA5;
  Bmp085Device *bmp085Device;
  TemperatureSensor *tSens;
  PressureSensor *pSens;

  keyPanel = new KeyPanel();
  scheduler = new Scheduler();
  acquaA5 = new BoardAcquaA5();
  display = new Winstar(*keyPanel, *scheduler, *acquaA5);

  keyPanel->set_event_filename(KEY_EVENT_DEVICE);

  shared_ptr < MenuActionVisitor > dw(new DisplayVisitor(*display));

  display->reset();
  keyPanel->start();

  bmp085Device = new Bmp085Device(*acquaA5);
  tSens = new TemperatureSensor(*bmp085Device, string("Temperature"));
  pSens = new PressureSensor(*bmp085Device, string("Pressure"));
  pSens->setAltituteCalibration(354.0);

  tSens->update();
  pSens->update();

  sleep(1);

  menu = new HomerMenu(*keyPanel, *scheduler, *tSens, *pSens);
  menu->addActionVisitor(dw);

  keyPanel->start();

  LOG4CPLUS_INFO(logger, "homerd started");
  while (true) {
    this_thread::sleep_for(std::chrono::seconds(10));
    // FIXME: condwait in sysinfo per exit
    if (keyPanel->get_key_counter() > 999999999)
      break;
  }

  keyPanel->stop();
  sleep(1);

  delete (menu);
  delete (display);
  delete (acquaA5);
  delete (scheduler);
  delete (keyPanel);

  LOG4CPLUS_INFO(logger, "homerd exit");
  return 0;
}
