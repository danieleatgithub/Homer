/*
 * CsvSensorDecorator.hpp
 *
 *  Created on: 22/dic/2016
 *      Author: daniele
 */

#ifndef CSVSENSORDECORATOR_HPP_
#define CSVSENSORDECORATOR_HPP_

//#include <sensor/Sensor.hpp>
#include <sensor/SensorDecorator.hpp>
namespace homerio {
class Sensor;

class CsvSensorDecorator : public SensorDecorator {

 public:
  CsvSensorDecorator(Sensor& sensor)
      : SensorDecorator(sensor) {
  }
  ;
  ~CsvSensorDecorator() {
  }
  ;

  const string getString() const {
    return (_sensor.getString() + ",");
  }

};

} /* namespace homerio */

#endif /* CSVSENSORDECORATOR_HPP_ */
