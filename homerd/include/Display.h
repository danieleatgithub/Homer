#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <string>
#include "KeyPanel.h"
#include "Calendar.h"

namespace homerio {
class GpioPin;
} /* namespace homerio */

using namespace std;
using namespace calendar;

namespace homerio {

class Display {

  private:
    virtual void init();
    unsigned int key_light_delay;
    unsigned int light_remain_ms;
    CalendarEvent timedLightOff;

  protected:
    int fd;
    uint8_t address;
    string bus;
    string rst;
    int write_usleep;

    string backlight;
    GpioPin* backlight_pin;
    GpioPin* reset_pin;
    bool backlight_state;
    virtual int device_init() {
        return (0);
    }
    ;

  public:

    Display(const char *bus);
    Display(const char *bus, const char *rst, const char *backlight);
    virtual ~Display();
    virtual int dpy_open();
    virtual int dpy_close();
    int dpy_putchar(unsigned char ch);
    int dpy_puts(const char *str);
    int reset();
    void key_attach(KeyPanel &key_panel);

    virtual int set_state(bool state) {
        return (0);
    }
    ;
    virtual bool is_display_on() {
        return (true);
    }
    ;
    virtual bool is_cursor_on() {
        return (true);
    }
    ;
    virtual int get_contrast() {
        return (0);
    }
    ;
    virtual int set_contrast(int value) {
        return (0);
    }
    ;
    virtual int set_cursor_on(bool state) {
        return (0);
    }
    ;
    virtual int set_cursor_blink(bool state) {
        return (0);
    }
    ;
    int set_backlight(bool state);
//    int set_backlight(bool state, unsigned int delay_ms);
    bool is_backlight_on();
    virtual int set_two_lines() {
        return (0);
    }
    ;
    virtual int set_one_line() {
        return (0);
    }
    ;
    virtual int set_double_height() {
        return (0);
    }
    ;
    virtual bool is_two_lines() {
        return (true);
    }
    ;
    virtual int clear() {
        return (0);
    }
    ;
    virtual int home() {
        return (0);
    }
    ;
    virtual int line2_home() {
        return (0);
    }
    ;
    virtual int shift_line() {
        return (0);
    }
    ;
    virtual int shift_cursor() {
        return (0);
    }
    ;
    virtual int set_insert_mode() {
        return (0);
    }
    ;
    virtual int set_overwrite_mode() {
        return (0);
    }
    ;

    virtual int write_data(unsigned char data) {
        return (0);
    }
    ;
};
}
#endif
