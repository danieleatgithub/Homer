#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <string>
#include "KeyPanel.hpp"
#include "Scheduler.hpp"
#include <HwLayer.hpp>


using namespace std;
using namespace shd;

namespace homerio {
class GpioPin;

class Display {

  private:
    unsigned int key_light_delay;
    unsigned int light_remain_ms;
    Task timedLightOff;
    KeyPanel& keyPanel;
    Scheduler& scheduler;
    Registration keyPanel_reg;

  protected:
    uint8_t address;
    string bus;
    string rst;
    int write_usleep;
    I2cBus& i2cBus;

    string backlight;
    GpioPin* backlight_pin;
    GpioPin* reset_pin;
    bool backlight_state;
    virtual int device_init() = 0;

  public:

    Display(KeyPanel &key_panel, Scheduler &scheduler,I2cBus& i2cBus, GpioPort& rst, GpioPort& backlight);
    virtual ~Display();

    int dpy_putchar(unsigned char ch);
    int dpy_puts(const char *str);
    int set_backlight(bool state);
    bool is_backlight_on();

    virtual int reset() = 0;
    virtual int set_state(bool state) = 0;
    virtual bool is_display_on() = 0;
    virtual bool is_cursor_on() = 0;
    virtual int get_contrast() = 0;
    virtual int set_contrast(int value) = 0;
    virtual int set_cursor_on(bool state) = 0;
    virtual int set_cursor_blink(bool state) = 0;


    virtual int set_two_lines() = 0;
    virtual int set_one_line() = 0;
    virtual int set_double_height() = 0;
    virtual bool is_two_lines() = 0;
    virtual int clear() = 0;
    virtual int home() = 0;
    virtual int line2_home() = 0;
    virtual int shift_line() = 0;
    virtual int shift_cursor() = 0;
    virtual int set_insert_mode() = 0;
    virtual int set_overwrite_mode() = 0;
    virtual int write_data(unsigned char data) = 0;
};
}
#endif
