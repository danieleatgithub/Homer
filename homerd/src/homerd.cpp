/*
 ============================================================================
 Name        : homerd.cpp
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Hello World in C++,
 ============================================================================
 */

#include <Display.h>
#include <fcntl.h>
#include <Fakedisplay.h>
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

using option::Option;
using option::Descriptor;
using option::Parser;
using option::Stats;
using option::ArgStatus;
using namespace std;
using namespace log4cplus;
using namespace homerio;

// FIXME get from properties
#define DAEMON_NAME "homerd"
#define ONEWIRE_0_PIN "PA01"
#define I2C_0_DAT_PIN "PA30"
#define I2C_0_CLK_PIN "PA31"
#define I2C_BUS "/dev/i2c-0"
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define LCD_BACKLIGHT_PIN "PA03"
#define LCD_RESET_PIN "PA02"
#define BUT_UP_PIN ""
#define BUT_DOWN_PIN ""
#define BUT_RIGHT_PIN ""
#define BUT_LEFT_PIN ""
#define BUT_ENTER_PIN ""

#define no_argument 0
#define required_argument 1
#define optional_argument 2


#define HOMERD_DEF_PROPS  "/etc/default/homerd.properties"
#define HOMERD_DEF_OUT_REDIR "/var/log/homerd_out.log"
#define HOMERD_DEF_ERR_REDIR "/var/log/homerd_err.log"

enum optionIndex {
    UNKNOWN, HELP, DAEMON, RSTDERR, RSTDOUT, PROPS
};
// clang-format off
// *INDENT-OFF*
const option::Descriptor usage[] = {
    { UNKNOWN, 0, "",        "", option::Arg::None, "USAGE: homerd [options]\n\nOptions:" },
    {    HELP, 0, "",    "help", option::Arg::None, "  --help  \tPrint usage and exit."   },
    {  DAEMON, 0, "d", "daemon", option::Arg::None, "  --daemon  \tStart as daemon."      },
    { RSTDERR, 0, "e", "err", option::Arg::Optional,"  --err, -e  \tredirect stderr to file." },
    { RSTDOUT, 0, "o", "out",  option::Arg::Optional, "  --out, -o  \tredirect stdout to file."},
    { PROPS,   0, "c", "conf", option::Arg::Optional, "  --conf, -c  \tconfiguration file."},
    { 0, 0, 0, 0, 0, 0 }
};
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
    option::Option *options =
        (option::Option *)calloc(stats.options_max, sizeof(option::Option));
    option::Option *buffer =
        (option::Option *)calloc(stats.buffer_max, sizeof(option::Option));
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
            exit(EXIT_FAILURE);
        }
        // We got a good pid, Close the Parent Process
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }
        // Change File Mask
        umask(0);
        // Create a new Signature Id for our child
        sid = setsid();
        if (sid < 0) {
            exit(EXIT_FAILURE);
        }
        // Change Directory
        // If we cant find the directory we exit with failure.
        if ((chdir("/tmp")) < 0) {
            exit(EXIT_FAILURE);
        }
        // Close Standard File Descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
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

    Display *display;
    string ip;
    bool run = true;
    Sysinfo sysinfo = Sysinfo::get_instance();
    initialize();

#if defined OFF_LINE_TARGET
    /*
     * Overwrite setting for emulation target
     */
    log4cplus::helpers::Properties props;
    PropertyConfigurator target_config(props_file);
    props = target_config.getProperties();
    props.setProperty("appender.ONFILE1.File","/wks/" + props.getProperty("appender.ONFILE1.File"));
    props.setProperty("appender.ONFILE2.File","/wks/" + props.getProperty("appender.ONFILE2.File"));
    PropertyConfigurator config(props);
    config.configure();
    display = new FakeDisplay(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
#else
    PropertyConfigurator config(props_file);
    config.configure();
    display = new Winstar(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
#endif

    Logger logger = Logger::getRoot();

    ip = sysinfo.get_local_ip("eth0");
    display->dpy_open();
    display->set_backlight(true);
    display->dpy_puts("Homer");  // 7
    display->line2_home();
    display->dpy_puts(ip.c_str());
    display->set_backlight(false, 10000);
    LOG4CPLUS_INFO(logger, "homerd started");

    for (int i = 0; i < 300; i++) {
        printf("xxx %d\n", i);
        sleep(1);
    }

    display->dpy_close();
    delete (display);
    LOG4CPLUS_INFO(logger, "homerd stopped");
    return 0;
}
