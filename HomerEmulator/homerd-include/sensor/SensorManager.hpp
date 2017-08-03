/*
 * SensorManager.h
 * - Collection of all sensors
 * - poll update
 * - collect data
 *  Created on: 16/gen/2017
 *      Author: daniele
 */

#ifndef SENSORMANAGER_HPP_
#define SENSORMANAGER_HPP_
#include <Scheduler.hpp>
#include <sensor/Sensor.hpp>
#include <sensor/CsvSensorDecorator.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <mutex>
#include <Scheduler.hpp>
#include <homerd.h>
#include <chrono>

using namespace commodities;
using namespace log4cplus;

namespace homerio {

class SensorManager {
 private:
  Scheduler& _shd;
  map<uint32_t, Sensor&> sensors;
  std::mutex mutex_sensors;
  Logger _csv;
  Task update_task;
  string last_update_csv;
  chrono::system_clock::time_point now;
  void update() {
    std::unique_lock < std::mutex > lock(mutex_sensors);
    last_update_csv.clear();
    now = chrono::system_clock::now();
    for (auto s : sensors) {
      s.second.update(now);
      last_update_csv += CsvSensorDecorator(s.second).getString();
    }
  }

 public:
  SensorManager(Scheduler &shd, const chrono::system_clock::duration period =
                    std::chrono::seconds(60))
      : _shd(shd),
        _csv(Logger::getInstance(LOGSENSORS)) {
    update_task.setPeriod(period);
    update_task.setCallback([&]() {
      this->update();
      LOG4CPLUS_INFO(_csv, last_update_csv);
    });
  }
  ;
  ~SensorManager() {
    stop();
  }
  ;
  void add(Sensor& s) {
    std::unique_lock < std::mutex > lock(mutex_sensors);
    sensors.insert(std::pair<uint32_t, Sensor&>(s.getId(), s));
  }

  void start() {
    update();
    _shd.ScheduleEvery(update_task);
  }
  void stop() {
    _shd.ScheduleCancel(update_task);
  }

  const chrono::system_clock::duration& getUpdatePeriod() const {
    return update_task.getPeriod();
  }

  void setUpdatePeriod(const chrono::system_clock::duration& updatePeriod) {
    update_task.setPeriod(updatePeriod);
  }
};

} /* namespace homerio */

#endif /* SENSORMANAGER_HPP_ */
