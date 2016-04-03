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
#include "Observer.h"
#include <linux/input.h>


using namespace obs;

namespace homerio {


enum Button_e {
	BUTTON_ENTER = KEY_ENTER,
	BUTTON_UP = KEY_UP,
	BUTTON_DOWN = KEY_DOWN,
	BUTTON_LEFT = KEY_LEFT,
	BUTTON_RIGHT = KEY_RIGHT
};
class KeyButton {
private:
	struct input_event key;
	std::chrono::milliseconds press_time;
	bool valid;

public:
	KeyButton() { valid = false; }
	void load_event(struct input_event &ev) {
		key = ev;
		valid = false;
	};
	void validate_event(struct input_event &ev) {
		valid = true;
		if(key.code == 0) {
//			press_time = std::chrono::duration_cast<std::chrono::milliseconds>((long long)(ev.time.tv_sec - key.time.tv_sec) * 1000 + (ev.time.tv_usec - key.time.tv_usec)/1000));
		}
	}
	bool isPressEvent() { return key.value == 1; }
	const Button_e get_key() { return ((Button_e) key.code); }
	std::chrono::milliseconds get_press_ms() { return(press_time); }


};

class KeyPanel {

  private:
    const char * dev;
    KeyButton key;
    obs::Subject<void (KeyButton& k )> key_obs;
    Registration reg_obs;

    bool running;
    std::thread key_thread;
    void key_thread_reader();
    unsigned int fd;
    unsigned int select_to;
    unsigned int key_counter;

  public:
    KeyPanel(const char *dev);
    ~KeyPanel();
    void key_attach(std::function<void (KeyButton& k)>);

    int start();
    int stop();
    unsigned int get_key_counter() { return(key_counter); }
};

}

#endif /* KEYPANEL_H_ */
