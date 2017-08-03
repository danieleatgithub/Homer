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
#include <EmulatedDevices.hpp>

namespace homeremulator {

EmulatedDevices::EmulatedDevices(BoardEmulated& _board)
    : board(_board) {
  temperature_register();
  barometric_register();
  current_register();
  voltage_register();
  power_register();
  rsense_register();
  shunt_register();
  hs11rh_register();
  hs11cycles_register();
  hs11frequency_register();
  hs11samplems_register();

}

EmulatedDevices::~EmulatedDevices() {
}

void EmulatedDevices::temperature_register() {
  board.getEmulatedSysFs().reg_read(
      temperature_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/temp0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vt = 200;
        vt+=5;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::barometric_register() {
  board.getEmulatedSysFs().reg_read(
      barometric_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/pressure0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vp = 97052-50;
        vp+=50;
        sprintf((char *)buffer,"%d",vp);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::current_register() {
  board.getEmulatedSysFs().reg_read(
      current_reg,
      string(
          "/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/curr1_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 1655;
        va+=50;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::voltage_register() {
  board.getEmulatedSysFs().reg_read(
      voltage_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/in1_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 4780;
        va+=50;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::power_register() {
  board.getEmulatedSysFs().reg_read(
      power_reg,
      string(
          "/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/power1_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 7860000;
        va+=50000;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::rsense_register() {
  board.getEmulatedSysFs().reg_read(
      rsense_reg,
      string("/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/in0_input"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 17;
        va+=5;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::shunt_register() {
  board.getEmulatedSysFs().reg_read(
      shunt_reg,
      string(
          "/sys/class/i2c-adapter/i2c-0/0-([0-9]+)/hwmon/hwmon0/shunt_resistor"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int va = 10000;
        va+=0;
        sprintf((char *)buffer,"%d",va);
        *ret = (int)strlen((char *)buffer);
      });
}

void EmulatedDevices::hs11rh_register() {
  board.getEmulatedSysFs().reg_read(
      hs11rh_reg,
      string("/sys/bus/iio/devices/iio:device([0-9]+)/in_humidityrelative_raw"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vt = 5431;
        vt+=45;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}

void EmulatedDevices::hs11cycles_register() {
  board.getEmulatedSysFs().reg_read(
      hs11cycles_reg, string("/sys/bus/iio/devices/iio:device([0-9]+)/cycles"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
      static int vt = 60;
      vt+=5;
      sprintf((char *)buffer,"%d",vt);
      *ret = (int)strlen((char *)buffer);
    });
}
void EmulatedDevices::hs11frequency_register() {
  board.getEmulatedSysFs().reg_read(
      hs11frequency_reg,
      string("/sys/bus/iio/devices/iio:device([0-9]+)/frequency"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vt = 6670;
        vt+=45;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::hs11samplems_register() {
  board.getEmulatedSysFs().reg_read(
      hs11samplems_reg,
      string("/sys/bus/iio/devices/iio:device([0-9]+)/sample_ms"),
      [&] (int filedes,void *buffer, size_t size, const char *fname, int *ret) {
        // TODO: Read emulation data from csv
        static int vt = 200;
        vt+=0;
        sprintf((char *)buffer,"%d",vt);
        *ret = (int)strlen((char *)buffer);
      });
}
void EmulatedDevices::start() {
}
void EmulatedDevices::stop() {
  // Observers will be unregisterd in destructor
}

}
/* namespace homeremulator */
