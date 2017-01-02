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

//  const double getNumeric() {
//    return (_sensor.getNumeric());
//  }
  const string get() const {
    return (_sensor.get());
  }
};

} /* namespace homerio */
#endif /* SENSORDECORATOR_HPP_ */
