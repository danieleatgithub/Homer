/*
 * homertool.cpp
 *
 *  Created on: 12/feb/2016
 *      Author: daniele
 */



#include <Display.h>
#include <fcntl.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <optionparser.h>
#include <stdio.h>
#include <Sysinfo.h>
#include <unistd.h>
#include <Winstar.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <linux/input.h>

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
#define BUT_ENTER_PIN "PA06"
#define HOMERD_DEF_PROPS  "/etc/default/homerd.properties"

#define no_argument 0
#define required_argument 1
#define optional_argument 2
enum optionIndex {
    UNKNOWN, HELP, PROPS, DPYTEST, KEYTEST
};



struct input_event ev;
struct input_id eii;

// clang-format off
// *INDENT-OFF*
const option::Descriptor usage[] = {
    { UNKNOWN, 0, "",         "", option::Arg::None, "USAGE: homerd [options]\n\nOptions:" },
    {    HELP, 0, "h",    "help", option::Arg::None, "  --help  \tPrint usage and exit."   },
    { PROPS,   0, "c",    "conf", option::Arg::Optional, "  --conf, -c  \tconfiguration file."},
    { DPYTEST, 0, "d", "dpytest", option::Arg::Optional, "  --dpytest, -d  \tdisplay test [1=simple]."},
    { KEYTEST, 0, "k", "keytest", option::Arg::Optional, "  --keytest, -k  \tkey test [1=on stdout 2=on display"},
    { 0, 0, 0, 0, 0, 0 }
};
// *INDENT-ON*
// clang-format on

const char *props_file = HOMERD_DEF_PROPS;
int   dpy_test= 0;
int   key_test= 0;

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
        case PROPS:
            if (opt.arg)
                props_file = opt.arg;
            break;
        case DPYTEST:
            if (opt.arg)
                dpy_test = atoi(opt.arg);
            else
                dpy_test = 1;
            break;
        case KEYTEST:
            if (opt.arg)
                key_test = atoi(opt.arg);
            else
                key_test = 1;
            break;
        case UNKNOWN:
            // not possible because Arg::Unknown returns ARG_ILLEGAL
            // which aborts the parse with an error
            break;
        }
    }


    Display *display;
    string ip;
    bool run = true;
    Sysinfo sysinfo = Sysinfo::get_instance();
    initialize();
    PropertyConfigurator config(props_file);
    config.configure();
    Logger logger = Logger::getRoot();


    switch(dpy_test) {
    case 1: {
        display = new Winstar(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
        cout << "start";
        ip = sysinfo.get_local_ip("eth0");
        display->dpy_open();
        display->set_backlight(true);
        display->dpy_puts("Homer");  // 7
        display->line2_home();
        display->dpy_puts(ip.c_str());
        display->set_backlight(false, 10000);
        LOG4CPLUS_INFO(logger, "homerd started");
        display->dpy_close();
        delete (display);
        LOG4CPLUS_INFO(logger, "homerd stopped");
        cout << "end";
        break;
    }
    default:
        break;
    }


    switch(key_test) {
    case 1: {
        int ffd = open("/dev/input/event0", O_RDONLY);
        struct timeval *tm;
        int key_code;
        int size = sizeof(ev);
        int rt;
        while(true) {
            rt = read(ffd, &ev, size);
            printf("code=%d type=%d value=%d s=%d u=%d \n", ev.code,ev.type,ev.value,ev.time.tv_sec,ev.time.tv_usec);
        }
        break;
    }
    case 2: {
        int ffd = open("/dev/input/event0", O_RDONLY);
        display = new Winstar(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
        display->dpy_open();
        display->set_backlight(true);
        struct timeval *tm;
        int key_code;
        int size = sizeof(ev);
        int rt;
        char buf[100];
        int stop_flag = 3;
        display->clear();
        snprintf(buf,sizeof(buf),"Press %d enter to quit", stop_flag);
        display->dpy_puts(buf);
        while(stop_flag > 0) {
            rt = read(ffd, &ev, size);
            snprintf(buf,sizeof(buf),"c=%d t=%d v=%d", ev.code,ev.type,ev.value);
            if(ev.value != 0) {
                display->clear();
                display->dpy_puts(buf);
            } else {
                display->line2_home();
                display->dpy_puts(buf);
            }
            if(ev.code == KEY_ENTER ) {
                stop_flag --;
            }  else {
                if(ev.code != 0) stop_flag = 3;
            }
        }
        display->set_backlight(false);
        display->dpy_close();
        delete (display);
        break;
    }
    default:
        break;
    }


    return 0;
}
