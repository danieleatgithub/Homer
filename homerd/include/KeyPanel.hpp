/*
 * KeyPanel.h
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */

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
