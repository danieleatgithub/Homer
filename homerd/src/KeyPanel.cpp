/*
 * KeyPanel.cpp
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */



#include "KeyPanel.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <thread>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace log4cplus;

namespace homerio {

KeyPanel::KeyPanel(const char *dev) {
	this->dev = dev;
	this->running = false;
	this->fd = -1;
	this->select_to = 40 * 1000;
	this->key_counter = 0;
}
KeyPanel::~KeyPanel() {
	this->dev = 0;
}
void KeyPanel::key_thread_reader() {
	Logger logdev = Logger::getInstance(LOGDEVICE);
	this->running = true;
	struct input_event ev;
	struct input_id eii;
    struct timeval *tm;
    int key_code;
    int size = sizeof(ev);
    int rt;
    fd_set rfs;
    struct timeval tout;
    tout.tv_sec = 0;
    tout.tv_usec = (this->select_to);
	while(running) {
	    FD_ZERO(&rfs);
	    FD_SET(this->fd,&rfs);
	    if(select(this->fd + 1, &rfs, NULL, NULL, &tout)) {
	    	rt = read(this->fd, &ev, size);
	    	LOG4CPLUS_DEBUG(logdev, "code=" << ev.code <<
	    							"type=" <<  ev.type <<
									"value="<< ev.value <<
									"s=" << ev.time.tv_sec <<
									"u=" << ev.time.tv_usec);
	    	if(ev.value != 0) {
	    		this->key_counter++;
	    	}
	    } else {
	    	// timeout
	    }

	}
	LOG4CPLUS_DEBUG(logdev, "KeyPanel thread exit\n");
}
int KeyPanel::start() {
	Logger logdev = Logger::getInstance(LOGDEVICE);
	if(running) {
		LOG4CPLUS_ERROR(logdev, "KeyPanel thread already active\n");
		return(-1);
	}
	fd = open(this->dev, O_RDONLY);
	if(fd < 0) {
		LOG4CPLUS_ERROR(logdev, "unable to open "<< this->dev << "\n");
		return(-2);
	}
	this->key_thread = std::thread([&] { KeyPanel::key_thread_reader(); });
	LOG4CPLUS_DEBUG(logdev, "key reader started\n");
	return(0);
}
int KeyPanel::stop() {
	Logger logdev = Logger::getInstance(LOGDEVICE);
	if(!running) {
		LOG4CPLUS_WARN(logdev, "KeyPanel thread not active\n");
		return(1);
	}
	this->running = false;
	this->key_thread.join();
	close(this->fd);
	this->fd = -1;
	return(0);
}


}
