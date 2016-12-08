/*
 * KeyPanel.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

#include <KeyPanel.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;
using namespace log4cplus;

namespace homerio {

KeyPanel::KeyPanel(string& dev) {
  this->eventFilename = dev;
  this->running = false;
  this->fd = -1;
  this->keyCounter = 0;
}
KeyPanel::KeyPanel() {
  this->eventFilename = string("");
  this->running = false;
  this->fd = -1;
  this->keyCounter = 0;
}
KeyPanel::~KeyPanel() {
  stop();
}

void KeyPanel::keyThreadReader() {
  Logger logdev = Logger::getInstance(LOGDEVICE);
  this->running = true;
  struct input_event ev;
  int size = sizeof(ev);
  int nread = 0;
  while (running) {
    // blocking event reader
    nread = read(this->fd, &ev, size);
    if (nread != size) {
      LOG4CPLUS_WARN(
          logdev,
          "read size error (s=" << (running ? "true" : "false") << " r="
              << nread << "e=" << size << ")");
      if (running)
        continue;
    }
    if (!running)
      break;
    LOG4CPLUS_TRACE(
        logdev,
        "(" << keyCounter << ") code=" << ev.code << " type=" << ev.type
            << " value=" << ev.value << " s=" << ev.time.tv_sec << " u="
            << ev.time.tv_usec);

    switch (ev.type) {
      case EV_KEY:
        this->key.loadEvent(ev);
        LOG4CPLUS_DEBUG(logdev, "EV_KEY " << string(this->key));
        break;
      case EV_SYN:
        this->key.validateEvent(ev);
        keyCounter++;
        LOG4CPLUS_DEBUG(
            logdev,
            "EV_SYN (" << std::to_string(keyCounter) << string(") ")
                << string(this->key));
        if (this->key.isPressEvent()) {
          keyPressObs(this->key);
        } else {
          keyReleaseObs(this->key);
        }

        if (this->key.isLongEvent()) {
          keyLongPressObs(this->key);
        }
        break;
    }

  }
  LOG4CPLUS_DEBUG(logdev, "KeyPanel thread exit\n");
}
int KeyPanel::start() {
  Logger logdev = Logger::getInstance(LOGDEVICE);

  if (running || eventFilename.empty()) {
    LOG4CPLUS_ERROR(
        logdev,
        (running ? "Already running" : "")
            << (eventFilename.empty() ? "No event device" : "") << endl);
    return (-1);
  }

  if ((fd = open(this->eventFilename.c_str(), O_RDONLY)) < 0) {
    LOG4CPLUS_ERROR(
        logdev,
        (string("unable to open ") + this->eventFilename
            + string(strerror(errno))).c_str());
    throw std::runtime_error(
        (string("unable to open ") + this->eventFilename
            + string(strerror(errno))).c_str());
  }
  this->keyThread = std::thread([&] {KeyPanel::keyThreadReader();});
  LOG4CPLUS_DEBUG(logdev, "key reader started");
  return (0);
}
int KeyPanel::stop() {
  // FIXME: mutex on running
  if (running) {
    running = false;
    Logger logdev = Logger::getInstance(LOGDEVICE);
    // Native pthread_cancel due blocking read that consume less cpu then timed select
    pthread_cancel(keyThread.native_handle());
    keyThread.join();
    close (fd);
    fd = -1;
    LOG4CPLUS_DEBUG(logdev, "key reader stopped");
    return (1);
  }
  return (0);
}
void KeyPanel::keyAttach(obs::Registration& reg,
                         std::function<void(KeyButton& k)> f) {
  keyReleaseAttach(reg, f);
}
void KeyPanel::keyReleaseAttach(obs::Registration& reg,
                                std::function<void(KeyButton& k)> f) {
  reg = keyReleaseObs.registerObserver(f);
}
void KeyPanel::keyPressAttach(obs::Registration& reg,
                              std::function<void(KeyButton& k)> f) {
  reg = keyPressObs.registerObserver(f);
}
void KeyPanel::keyLongPressAttach(obs::Registration& reg,
                                  std::function<void(KeyButton& k)> f) {
  reg = keyLongPressObs.registerObserver(f);
}
const unsigned int KeyPanel::getKeyCounter() {
  return (keyCounter);
}

void KeyPanel::setEventFilename(const char *dev) {
  this->eventFilename = dev;
}
const string& KeyPanel::getEventFilename() {
  return (this->eventFilename);
}

}
