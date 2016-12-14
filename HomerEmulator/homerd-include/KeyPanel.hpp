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

#ifndef KEYPANEL_HPP_
#define KEYPANEL_HPP_

#include <chrono>
#include <thread>
#include <string>
#include "Observer.h"
#include <linux/input.h>
#include <map>
#include <KeyButton.hpp>

using namespace std;

namespace homerio {

class KeyPanel {

 private:
  KeyButton key;
  // Observers
  obs::Subject<void(KeyButton& k)> keyPressObs;  // Triggered on key press
  obs::Subject<void(KeyButton& k)> keyReleaseObs;  // Triggered on key released
  obs::Subject<void(KeyButton& k)> keyLongPressObs;  // Triggered on long time duration

  struct timeval tout;
  string eventFilename;
  bool running;
  std::thread keyThread;
  int fd;
  unsigned int keyCounter;

  /**
   * event reader
   */
  void keyThreadReader();

 public:
  KeyPanel(string& dev);
  KeyPanel();
  ~KeyPanel();
  void keyAttach(obs::Registration& reg, std::function<void(KeyButton& k)> f);
  void keyReleaseAttach(obs::Registration& reg,
                        std::function<void(KeyButton& k)> f);
  void keyPressAttach(obs::Registration& reg,
                      std::function<void(KeyButton& k)> f);
  void keyLongPressAttach(obs::Registration& reg,
                          std::function<void(KeyButton& k)> f);
  int start();
  int stop();

  const unsigned int getKeyCounter();

  void setEventFilename(const char *dev);
  const string& getEventFilename();
};

}

#endif /* KEYPANEL_HPP_ */
