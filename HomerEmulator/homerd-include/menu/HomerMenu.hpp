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

#ifndef HOMERMENU_HPP_
#define HOMERMENU_HPP_
#include <Menu.hpp>
#include <mutex>
#include <memory>
#include <Sysinfo.h>
#include <iostream>
#include <string>
#include <TemperatureSensor.hpp>
#include <BarometricSensor.hpp>
using namespace std;

namespace homerio {

/**
 * Homer Menu
 *
 */

class HomerMenu {
 private:

  Task timedHome;
  shared_ptr<MenuComponent> active_element;
  KeyPanel &keyPanel;
  Scheduler& scheduler;
  TemperatureSensor& tsens;
  BarometricSensor& psens;
  obs::Registration rkeyPanel, rWrDisplay;
  Sysinfo sysinfo = Sysinfo::get_instance();

  KeyButton nokey;
  std::mutex mutex;

  SimpleMenuElement menuIP = SimpleMenuElement(
      "Homer", "IP:" + sysinfo.get_local_ip("eth0"));
  SimpleMenuElement menuCpu = SimpleMenuElement("cpu");
  SimpleMenuElement subRoot = SimpleMenuElement("root");
  SimpleMenuElement subSys = SimpleMenuElement("system");
  SimpleMenuElement subAlarm = SimpleMenuElement("alarms");
  SimpleMenuElement subSens = SimpleMenuElement("sensors");

  shared_ptr<SubMenu> root = std::make_shared < SubMenu > (subRoot);
  shared_ptr<MenuLeaf> welcome = std::make_shared < MenuLeaf > (menuIP);
  shared_ptr<SubMenu> system = std::make_shared < SubMenu > (subSys);
  shared_ptr<MenuLeaf> cpu = std::make_shared < MenuLeaf > (menuCpu);
  shared_ptr<SubMenu> sensors = std::make_shared < SubMenu > (subSens);
  shared_ptr<MenuLeaf> temperature;
  shared_ptr<MenuLeaf> pressure;
  shared_ptr<SubMenu> alarms = std::make_shared < SubMenu > (subAlarm);

  MenuMoveVisitor mv;
  vector<shared_ptr<MenuActionVisitor>> visitors;

  void leave(KeyButton& k) {
    for (auto v : visitors) {
      active_element->execLeave(*v, k);
    }
  }
  void enter(KeyButton& k) {
    for (auto v : visitors) {
      active_element->execEnter(*v, k);
    }
  }
  void click(KeyButton& k) {
    for (auto v : visitors) {
      active_element->execClick(*v, k);
    }
  }

 public:
  HomerMenu(KeyPanel& kpl, Scheduler& sch, TemperatureSensor& _tsens,
            BarometricSensor& _psens)
      : keyPanel(kpl),
        scheduler(sch),
        tsens(_tsens),
        psens(_psens),
        mv(root) {
    Logger loghomer = Logger::getInstance(LOGHOMERD);

    temperature = std::make_shared < MenuLeaf > (tsens);
    pressure = std::make_shared < MenuLeaf > (psens);

    root->add(welcome);
    sensors->add(temperature);
    sensors->add(pressure);
    system->add(cpu);
    system->add(alarms);
    root->add(sensors);
    root->add(system);

    root->home();
    active_element = root->getActiveElement();

    // Timer on no key press reset menu to Home
    timedHome.setCallback([&] () {
      leave(nokey);
      mutex.lock();
      try {
        active_element = mv.home();
      } catch (MenuEmptyException& e) {
        LOG4CPLUS_WARN(loghomer, "timedHome MenuEmptyException " << e.what());
      } catch (MenuException& e) {
        LOG4CPLUS_ERROR(loghomer, "timedHome MenuException " << e.what());
      }
      mutex.unlock();
      enter(nokey);
    });

    // Attach key handler
    keyPanel.keyAttach(rkeyPanel, [&] ( KeyButton& k ) {
      shared_ptr<MenuComponent> temp;
      scheduler.ScheduleCancel(timedHome);
      if(k.getKey() == BUTTON_ENTER) {
        click(k);
      } else {
        leave(k);
        mutex.lock();
        try {
          active_element = active_element->execMove(mv,k);
        } catch (MenuEmptyException& e) {
          LOG4CPLUS_WARN(loghomer, "timedHome MenuEmptyException " << e.what());
        } catch (MenuException& e) {
          LOG4CPLUS_ERROR(loghomer, "timedHome MenuException " << e.what());
        }

        mutex.unlock();
        enter(k);
      }
      scheduler.ScheduleAfter(std::chrono::seconds(30),timedHome);
    });
    // Display welcome immediatly
    scheduler.ScheduleAfter(std::chrono::milliseconds(100), timedHome);

  }
  virtual ~HomerMenu() {
  }

  void addActionVisitor(const shared_ptr<MenuActionVisitor>& visitor) {
    visitors.push_back(visitor);
  }

};

} /* namespace homerio */

#endif /* HOMERMENU_HPP_ */
