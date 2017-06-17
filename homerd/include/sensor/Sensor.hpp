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

#ifndef SENSOR_HPP_
#define SENSOR_HPP_
#include <MenuAble.hpp>
#include <IDGenerator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <map>

using namespace std;
using namespace commodities;
using namespace log4cplus;

namespace homerio {

class Sensor {
 protected:
  string name;
  uint32_t id;
  int precision = 4;
  int scale = 0;
  map<int, string> units;
  Logger _loghomer;

 public:

  Sensor() {
    id = IDGenerator::get_istance().getId();
    Logger _loghomer = Logger::getInstance(LOGHOMERD);
    // Initialization in derived class it's raccomanded
    precision = 0;
    scale = 0;
    units[0] = "";
  }
  Sensor(const string& _name)
      : name(_name) {
    id = IDGenerator::get_istance().getId();
  }
  virtual ~Sensor() {
  }

  virtual const string getString() const = 0;
  virtual const double getDouble() const = 0;
  virtual void update() = 0;

  const string& getName() const {
    return name;
  }

  void setName(const string& name) {
    this->name = name;
  }

  uint32_t getId() const {
    return id;
  }

  /**
   * Same task has same id
   * @param lhs
   * @param rhs
   * @return
   */
  friend bool operator==(const Sensor& lhs, const Sensor& rhs) {
    return (lhs.id == rhs.id);
  }
  /**
   * Ordered by deadline first is the earliest
   * @param lhs
   * @param rhs
   * @return
   */
  friend bool operator<(const Sensor& lhs, const Sensor& rhs) {
    return (lhs.id < rhs.id);
  }

  int getPrecision() const {
    return precision;
  }

  void setPrecision(int precision) {
    this->precision = precision;
  }

  int getScale() const {
    return scale;
  }

  void setScale(int scale) {
    if (units.find(scale) == units.end()) {
      LOG4CPLUS_WARN(_loghomer, "scale not in range (" << scale << ")");
      return;
    }
    this->scale = scale;
  }
};

}

#endif /* SENSOR_HPP_ */
