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
#include <HwLayer.hpp>
#include <EmulatedHw.hpp>
#include <HomerEmulator.hpp>

#ifndef SRC_EMULATEDDEVICE_HPP_
#define SRC_EMULATEDDEVICE_HPP_

namespace homeremulator {

/**
 * Factory of all device emulation
 */
class EmulatedDevices {
 public:
  EmulatedDevices(BoardEmulated& _board);
  virtual ~EmulatedDevices();
  /**
   * start all emulation
   */
  void start();
  /**
   * stop all emulation
   */
  void stop();

 private:
  BoardEmulated& board;
  Registration barometric_reg;
  Registration temperature_reg;
  Registration current_reg;
  Registration voltage_reg;
  Registration power_reg;
  Registration rsense_reg;
  Registration shunt_reg;
  Registration hs11rh_reg;
  Registration hs11cycles_reg;
  Registration hs11frequency_reg;
  Registration hs11samplems_reg;
  Registration hih5030rh_reg;
  Registration hih5030scale_reg;

  void temperature_register();
  void barometric_register();
  void current_register();
  void voltage_register();
  void power_register();
  void rsense_register();
  void shunt_register();
  void hs11rh_register();
  void hs11cycles_register();
  void hs11frequency_register();
  void hs11samplems_register();
  void hih5030rh_register();
  void hih5030scale_register();
};

}
/* namespace homeremulator */

#endif /* SRC_EMULATEDDEVICE_HPP_ */
