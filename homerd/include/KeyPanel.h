/*
 * KeyPanel.h
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */

#ifndef KEYPANEL_H_
#define KEYPANEL_H_

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <stdio.h>
#include <unistd.h>
#include <thread>


namespace homerio {

class KeyPanel {

  private:
    const char * dev;
    bool running;
    std::thread key_thread;
    void key_thread_reader();
    unsigned int fd;
    unsigned int select_to;
    unsigned int key_counter;

  public:
    KeyPanel(const char *dev);
    ~KeyPanel();
    int start();
    int stop();
    unsigned int get_key_counter() { return(key_counter); }
};

}

#endif /* KEYPANEL_H_ */
