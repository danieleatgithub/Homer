/*
 * SensorDecorator.hpp
 *
 *  Created on: 22/dic/2016
 *      Author: daniele
 */

#ifndef SENSORDECORATOR_HPP_
#define SENSORDECORATOR_HPP_
#include <string>
#include <sensor/Sensor.hpp>
using namespace std;

namespace homerio {

/*
 * Abstract decorator for a sensor
 */
class SensorDecorator : public Sensor {
 public:
  SensorDecorator(Sensor& sensor)
      : _sensor(sensor) {
  }
  ;
  ~SensorDecorator() {
  }
  ;
  Sensor& _sensor;

  const string getString() const {
    return (_sensor.getString());
  }
  const double getDouble() const {
    return (_sensor.getDouble());
  }
  void update(chrono::system_clock::time_point time_point) {
    _sensor.update(time_point);
  }

};

} /* namespace homerio */
#endif /* SENSORDECORATOR_HPP_ */
