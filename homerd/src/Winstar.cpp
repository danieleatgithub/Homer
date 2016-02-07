#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Winstar.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>

using namespace std;
using namespace log4cplus;

namespace homerio {

const unsigned int freqency_3_volts[8] = { 122, 131, 144, 161, 183, 221, 274,
                                           347
                                         };
const unsigned int freqency_5_volts[8] = { 120, 133, 149, 167, 192, 227, 277,
                                           347
                                         };

void Winstar::init() {
    this->entry_mode = 0;
    this->display_mode = 0;
    this->function_set = 0;
    this->ddram_addr = 0;
    // Normal instruction set (IS=0)
    this->cursor_display_shift = 0;
    // Extended instruction set (IS=1)
    this->bias_osc_frequency_adj = 0;
    this->icon_ram_address = 0;
    this->pow_icon_contrast = 0;
    this->follower = 0;
    this->contrast_set = 0;
    this->address = WINSTAR_I2C_ADD;

}
Winstar::Winstar(const char *bus) :
    Display(bus) {
    init();
}

Winstar::Winstar(const char *bus, const char *rst, const char *backlight) :
    Display(bus, rst, backlight) {
    init();
}

Winstar::~Winstar() {
}

int Winstar::clear() {
    return (write_cmd(WSTAR_CLEAR_DISPLAY_CMD));
}

int Winstar::home() {
    return (write_cmd(WSTAR_RETURN_HOME_CMD));
}
int Winstar::set_state(bool state) {
    int ret = -1;
    unsigned char oldreg = display_mode;
    (state ? display_mode |= WSTAR_DISPLAY_STATE : display_mode &=
                                 ~WSTAR_DISPLAY_STATE);
    ret = write_cmd(display_mode);
    if (ret < 0)
        display_mode = oldreg;
    return (ret);
}
bool Winstar::is_display_on() {
    return ((display_mode & WSTAR_DISPLAY_STATE) != 0);
}
int Winstar::set_cursor_on(bool state) {
    (state ? display_mode |= WSTAR_DISPLAY_CURSOR : display_mode &=
                                 ~WSTAR_DISPLAY_CURSOR);
    return (write_cmd(display_mode));
}
int Winstar::set_cursor_blink(bool state) {
    (state ? display_mode |= WSTAR_DISPLAY_BLINK : display_mode &=
                                 ~WSTAR_DISPLAY_BLINK);
    return (write_cmd(display_mode));
}
bool Winstar::is_cursor_on() {
    return ((display_mode & WSTAR_DISPLAY_CURSOR) != 0);
}
int Winstar::get_contrast() {
    return ((int) (((pow_icon_contrast & WSTAR_POW_ICON_CONTRAST_CNTH_MASK) << 4)
                   | (contrast_set & WSTAR_CONTRAST_LOW_MASK)));
}
int Winstar::set_contrast(int value) {
    int ret = -2;
    unsigned char oldreg;
    set_extended_mode();
    if (value > CONTRAST_MAX || value < 0)
        return (-2);
    oldreg = contrast_set;
    contrast_set = (value & WSTAR_CONTRAST_LOW_MASK);
    ret = write_cmd(contrast_set);
    if (ret < 0) {
        contrast_set = oldreg;
        return (ret);
    }
    oldreg = pow_icon_contrast;
    pow_icon_contrast |= ((value >> 4) & WSTAR_POW_ICON_CONTRAST_CNTH_MASK);
    ret = write_cmd(pow_icon_contrast);
    if (ret < 0) {
        pow_icon_contrast = oldreg;
        return (ret);
    }
    return (ret);
}

int Winstar::set_insert_mode() {
    entry_mode |= WSTAR_ENTRY_MODE_SHIFT_ON;
    return (write_cmd(entry_mode));
}
int Winstar::set_overwrite_mode() {
    entry_mode &= ~WSTAR_ENTRY_MODE_SHIFT_ON;
    return (write_cmd(entry_mode));
}
int Winstar::shift_line() {
    set_normal_mode();
    cursor_display_shift |= WSTAR_CUR_DPY_SHIFT_SCREEN;
    return (write_cmd(cursor_display_shift));
}
int Winstar::shift_cursor() {
    set_normal_mode();
    cursor_display_shift &= ~WSTAR_CUR_DPY_SHIFT_SCREEN;
    return (write_cmd(cursor_display_shift));
}
int Winstar::set_double_height() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set |= WSTAR_FUNCTION_ONE_LINE_5X16;
    return (write_cmd(function_set));
}
int Winstar::set_two_lines() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
    function_set |= WSTAR_FUNCTION_TWO_LINE_5X8;
    return (write_cmd(function_set));
}
int Winstar::set_one_line() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
    return (write_cmd(function_set));
}
bool Winstar::is_two_lines() {
    return ((function_set & WSTAR_FUNCTION_TWO_LINE_5X8) != 0);
}
int Winstar::line2_home() {
    ddram_addr = WSTAR_DDRAM_CMD | WSTAR_DDRAM_LINE2;
    return (write_cmd(ddram_addr));
}
// private methods

int Winstar::device_init() {
    int ret;
    this->display_mode = WSTAR_DISPLAY_STATE | WSTAR_DISPLAY_CMD;
    this->function_set = WSTAR_FUNCTION_8BIT | WSTAR_FUNCTION_TWO_LINE_5X8
                         | WSTAR_FUNCTION_CMD;
    this->entry_mode = WSTAR_ENTRY_MODE_CMD;
    this->cursor_display_shift = WSTAR_CUR_DPY_SHIFT_RIGHT
                                 | WSTAR_CUR_DPY_SHIFT_SCREEN | WSTAR_CUR_DPY_SHIFT_CMD;
    this->ddram_addr = WSTAR_DDRAM_CMD;
    // Extended instruction set (IS=1)
    this->bias_osc_frequency_adj = 0x04 | WSTAR_BIAS_OSC_CMD;
    this->icon_ram_address = WSTAR_ICON_RAM_ADD_CMD;
    this->pow_icon_contrast = WSTAR_POW_ICON_CONTRAST_BOOST
                              | WSTAR_POW_ICON_CONTRAST_CMD;
    this->follower = FOLLOWER_DEFAULT | WSTAR_FOLLOWER_ON | WSTAR_FOLLOWER_CMD;
    this->contrast_set = CONTRAST_DEFAULT | WSTAR_CONTRAST_LOW_CMD;

    ret = write_cmd(function_set);
    ret = set_extended_mode();
    ret = write_cmd(bias_osc_frequency_adj);
    ret = write_cmd(contrast_set);
    ret = write_cmd(pow_icon_contrast);
    ret = write_cmd(follower);
    ret = write_cmd(display_mode);
    return (ret);
}
int Winstar::set_normal_mode() {
    function_set &= ~WSTAR_FUNCTION_EXTENDED;
    return (write_cmd(function_set));
}
int Winstar::set_extended_mode() {
    function_set |= WSTAR_FUNCTION_EXTENDED;
    return (write_cmd(function_set));
}
int Winstar::write_cmd(unsigned char data) {
    return (this->dpy_write(WSTAR_CMD, data));
}
int Winstar::write_data(unsigned char data) {
    return (this->dpy_write(WSTAR_DATA, data));
}
int Winstar::dpy_write(int type, uint8_t data) {
    unsigned char buffer[2];
    buffer[0] = (unsigned char) type;
    buffer[1] = data;
    Logger logdev = Logger::getInstance("homerd.device");
    LOG4CPLUS_DEBUG(logdev,"Winstar i2cset -y 0 0x " << std::hex << address << " 0x" <<  std::hex << type << " 0x" << std::hex << data);

    // FIXME: wait ready from device (blocking and no blocking)
    if (write(fd, buffer, 2) != 2) {
        printf("Error writing file: %s\n", strerror(errno));
        return -1;
    }
    fsync(fd);
    usleep(write_usleep);
    return (0);
}

}

