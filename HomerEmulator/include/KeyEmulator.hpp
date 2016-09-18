/*
 * KeyEmulator.hpp
 *
 * References
 * http://thiemonge.org/getting-started-with-uinput
 * https://web.archive.org/web/20120710121647/http://www.einfochips.com/download/dash_jan_tip.pdf
 * http://www.linux.it/~rubini/docs/input26/input26.html
 * http://git.nosuchfile.org/?p=uinput-sample.git;a=blob_plain;f=uinput-sample.c;hb=HEAD
 *
 *  Created on: 07/ago/2016
 *      Author: daniele
 */

#ifndef KEYEMULATOR_HPP_
#define KEYEMULATOR_HPP_
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <dirent.h>
#include <string>
#include <fcntl.h>
#include <thread>
#include <iostream>
#include <GL/freeglut.h>

#define SYS_PATH "/sys/class/input/"
#define EVT_PATH "/dev/input/"

#define KEY_EMULATOR_DIE(str, args...) do { \
        perror(str); \
        fprintf(stderr,"%s:%d\n",__FILE__,__LINE__); \
        exit(EXIT_FAILURE); \
    } while(0)





/*
 * Event emulator for keypanel, remap key from stdin to event for Homer KeyPanel
 * use linux module uinput
 *
 * KeyPanel Real Example
 *

[root@homer input]# ls -l
total 0
crw------- 1 root root 13, 64 Jan  1  1970 event0
[root@homer input]# evtest event0
Input driver version is 1.0.1
Input device ID: bus 0x19 vendor 0x1 product 0x1 version 0x100
Input device name: "ahb:apb:gpio_keys"
Supported events:
  Event type 0 (EV_SYN)
  Event type 1 (EV_KEY)
    Event code 28 (KEY_ENTER)
    Event code 103 (KEY_UP)
    Event code 105 (KEY_LEFT)
    Event code 106 (KEY_RIGHT)
    Event code 108 (KEY_DOWN)
Properties:
Testing ... (interrupt to exit)
Event: time 1470768867.077489, type 1 (EV_KEY), code 103 (KEY_UP), value 1
Event: time 1470768867.077489, -------------- SYN_REPORT ------------
Event: time 1470768867.350997, type 1 (EV_KEY), code 103 (KEY_UP), value 0
Event: time 1470768867.350997, -------------- SYN_REPORT ------------

Event: time 1470768876.142763, type 1 (EV_KEY), code 108 (KEY_DOWN), value 1
Event: time 1470768876.142763, -------------- SYN_REPORT ------------
Event: time 1470768876.484660, type 1 (EV_KEY), code 108 (KEY_DOWN), value 0
Event: time 1470768876.484660, -------------- SYN_REPORT ------------

Long click
Pressed
Event: time 1470769541.133427, type 1 (EV_KEY), code 103 (KEY_UP), value 1
Event: time 1470769541.133427, -------------- SYN_REPORT ------------
Released
Event: time 1470769545.002499, type 1 (EV_KEY), code 103 (KEY_UP), value 0
Event: time 1470769545.002499, -------------- SYN_REPORT ------------

*
* class usage example:
*
*
	Scheduler 	sch;
	HomerMenu 	homerMenu;
	KeyEmulator key_emulator;
	KeyPanel 	key_panel;

	key_emulator.start(); // Start keyboard Emulator
	key_panel.set_event_filename(key_emulator.getEvent().c_str());
	key_panel.start();  // Start keyPanel
	homerMenu.start(key_panel,sch); // Start Menu

	key_emulator.join();
*
* Mapped key:
* UP=8,DOWN=2,LEFT=4,RIGHT=6,ENTER=0
* Emulator special key
* '-' = Enable disable auto button release event
* '+' = If auto release is disabled send release event (For long press emulation)
* 'q' = quit the thread
*
*
*/

using namespace std;

class KeyEmulator {

private:
	// uinput device
	struct uinput_user_dev uidev;
	struct termios t;
	char sysfs_device_name[16];
    int                    fd; // fd of the uinput device
    struct input_event     evt;

    std::thread key_thread;
    bool running;
	string input;
	string event;
	bool key_pressed;
	unsigned int key_us_auto_release, stored_release;

	/**
	 * Set stdin without buffer (Note in eclipse console has no effect)
	 */
    void stdin_unbuffered()
    {
    	tcgetattr(STDIN_FILENO, &t);
    	t.c_lflag &= ~ICANON;
    	t.c_lflag &= ~ECHO;
    	tcsetattr(STDIN_FILENO, 0, &t);
    }
    /**
     * Restore normal stdin
     */
    void stdin_buffered()
    {
    	tcgetattr(STDIN_FILENO, &t);
    	t.c_lflag |= ICANON;
       	t.c_lflag |= ECHO;
    	tcsetattr(STDIN_FILENO, 0, &t);
    }
	/**
	 *  Open and initialize uinput driver
	 *  DIE on error
	 *
	 * @param name  is the given name to the input device we will create
	 * @param type (default = BUS_USB)
	 * @param vendor (default = 0xc0da)
	 * @param product (default = 0xcafe)
	 * @param version (default = 1)
	 * @param key_us_auto_release automatic key release in microseconds, if 0 release is sent on key '+' (default = 100000)
	 */
    void init(const char *name,__u16 type = BUS_USB,__u16 vendor = 0xc0da,__u16 product = 0xcafe,__u16 version = 1,unsigned int key_us_auto_release = 100000) {
    	DIR *pdir;
    	struct dirent *pent;
    	this->running = false;
    	this->key_pressed = false;
     	this->key_us_auto_release = key_us_auto_release;
    	this->stored_release = key_us_auto_release;
    	// Open the device that emulate user input
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

        // Configure event as real Homer generate
        // Die on error
        if(fd < 0)      							KEY_EMULATOR_DIE("error: open");
        if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)		KEY_EMULATOR_DIE("error: ioctl EV_KEY");
        if(ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)  	KEY_EMULATOR_DIE("error: ioctl EV_SYN");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_ENTER) < 0) KEY_EMULATOR_DIE("error: ioctl KEY_ENTER");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_UP) < 0)  	KEY_EMULATOR_DIE("error: ioctl KEY_UP");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_DOWN) < 0)  KEY_EMULATOR_DIE("error: ioctl KEY_DOWN");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_LEFT) < 0)  KEY_EMULATOR_DIE("error: ioctl KEY_LEFT");
        if(ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT) < 0) KEY_EMULATOR_DIE("error: ioctl KEY_RIGHT");

        // Device Creation
        memset(&uidev, 0, sizeof(uidev));
        strncpy(uidev.name, name, UINPUT_MAX_NAME_SIZE);
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor  = vendor;
        uidev.id.product = product;
        uidev.id.version = version;
        if(write(fd, &uidev, sizeof(uidev)) < 0) 	KEY_EMULATOR_DIE("error: write");
        if(ioctl(fd, UI_DEV_CREATE) < 0)   			KEY_EMULATOR_DIE("error: ioctl");
        memset(&sysfs_device_name, 0, sizeof(sysfs_device_name));
        ioctl(fd, UI_GET_SYSNAME(sizeof(sysfs_device_name)), sysfs_device_name);

        // Store info about the created device, search in sys path for the event created by the OS
        input = string(SYS_PATH)+string(sysfs_device_name);
        pdir = opendir(input.c_str());
        while ((pent = readdir(pdir)) != NULL) {
        	if (strstr(pent->d_name, "event") == pent->d_name) {
                  event = string(EVT_PATH) + string(pent->d_name);
                  break;
            }
        }
        if(event.empty()) KEY_EMULATOR_DIE("syspath event not found");
        (void)closedir(pdir);
    }

    /**
     * Send a key and after us_glitch the EV_SYN event
     * DIE on error
     *
     * @param type
     * @param code
     * @param value
     * @param event_us_pause ( deafult = 100 microseconds)
     */
    void k_write(__u16 type,__u16 code, __s32 value, unsigned int event_us_pause = 100) {
       memset(&evt, 0, sizeof(struct input_event));
       evt.type = type;
       evt.code = code;
       evt.value = value;
       if(write(fd, &evt, sizeof(struct input_event)) < 0)
        	KEY_EMULATOR_DIE("error: write");
       usleep(event_us_pause);
       memset(&evt, 0, sizeof(struct input_event));
       evt.type = EV_SYN;
       if(write(fd, &evt, sizeof(struct input_event)) < 0)
        	KEY_EMULATOR_DIE("error: write EV_SYN");
       usleep(event_us_pause);
    }

    /**
     * Thread key emulator
     * Mapped key:
     * UP=8,DOWN=2,LEFT=4,RIGHT=6,ENTER=0
     * Emulator special key
     * '-' = Enable disable auto button release event
     * '+' = If auto release is disabled send release event (For long press emulation)
     * 'q' = quit the thread
     *
     */
    __attribute__((deprecated)) void key_thread_emulator() {
        struct timeval tv;
        fd_set fds;
    	char ch;
    	__u16 current_key;
    	__s32 current_val;

        tv.tv_sec = 1; // Suspend timeout
        tv.tv_usec = 0;
    	if(running) KEY_EMULATOR_DIE("KeyEmulator Already running");
    	running = true;
    	cerr << "KeyEmulator thread STARTED NAV=8624 '-'=auto release toggle '+'=send release 'q'=Thread quit" << endl;
    	while(running) {
    		FD_ZERO(&fds);
    		FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    		select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    		if(FD_ISSET(STDIN_FILENO, &fds)) {
    			ch = std::cin.get();
				current_val = 1;
				switch(ch) {
					case '2':
						current_key = KEY_DOWN;
						break;
					case '8':
						current_key = KEY_UP;
						break;
					case '6':
						current_key = KEY_RIGHT;
						break;
					case '4':
						current_key = KEY_LEFT;
						break;
					case '0':
						current_key = KEY_ENTER;
						break;
					case '+':
						if(key_us_auto_release == 0) current_val = 0;
						else current_key = KEY_ESC;
						break;
					case '-':
						current_key = KEY_ESC;
						if(key_us_auto_release == 0) {
							key_us_auto_release = stored_release;
							cerr << "KeyEmulator: auto release active (" <<  key_us_auto_release << ")" << endl;
						} else {
							stored_release = key_us_auto_release;
							key_us_auto_release = 0;
							cerr << "KeyEmulator: auto release deactivated" << endl;
						}
						break;
					case 'q':
						running = false;
						break;
					default:
						current_key = KEY_ESC;
    			}
				// Write key events on the device
    			if(running && current_key != KEY_ESC) {
    				k_write(EV_KEY,current_key,current_val);
    				if(key_us_auto_release > 0) {
    					usleep(key_us_auto_release);
    					current_val = 0;
    					k_write(EV_KEY,current_key,current_val);
    				}
    			}

    		}
    	}
    	cerr << "KeyEmulator thread terminated" << endl;
    }

public:

    KeyEmulator() {
    	init("uinput-sample");
    }
    KeyEmulator(const char *name) {
    	init(name);
    }
    KeyEmulator(const char *name,__u16 type,__u16 vendor,__u16 product,__u16 version, unsigned int key_us_auto_release) {
    	init(name, type, vendor, product, version, key_us_auto_release);
    }
    ~KeyEmulator() {
//    	if(running) stop();
    	if(ioctl(fd, UI_DEV_DESTROY) < 0) KEY_EMULATOR_DIE("error: ioctl in ~KeyEmulator");
   	    close(fd);
    }
    void gl_key_event(unsigned char key, int x, int y,__s32 value) {
    	__u16 current_key = KEY_ESC;;
 	   switch (key) {
		case '2':
			current_key = KEY_DOWN;
			break;
		case '8':
			current_key = KEY_UP;
			break;
		case '6':
			current_key = KEY_RIGHT;
			break;
		case '4':
			current_key = KEY_LEFT;
			break;
		case '0':
			current_key = KEY_ENTER;
			break;
		case 'q':
			glutLeaveMainLoop();
			break;
		default:
	    	cerr << __PRETTY_FUNCTION__ << " Unmanaged key:" << key << endl;
			current_key = KEY_ESC;
 	   }
		// Write key events on the device
		if(current_key != KEY_ESC) {
			k_write(EV_KEY,current_key,value);
		}

    }

    void gl_key_press(unsigned char key, int x, int y) {
    	gl_key_event(key,x,y,1);
    }

    void gl_key_release(unsigned char key, int x, int y) {
    	gl_key_event(key,x,y,0);
    }

    int gl_start(unsigned int max_sync_ms = 10000) {
		int fd;
		int ret = -1;

		for(int i= 0; i< 100; i++) {
			  if((fd = open(getEvent().c_str(), O_RDONLY)) > 0) {
				  close(fd);
				  ret = 0;
				  break;
			  }
			  usleep(max_sync_ms); // 1ms
		}
		// fails if event not available in 1 second
		return(ret);

    }
    int gl_stop() {
    	return(0);
    }


   /**
     * Start text based emulator (Deprecated)
     *
     * @param max_sync_ms milliseconds to wait the device availability (default = 1000)
     * @return positive on success
     */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	__attribute__((deprecated)) int start(unsigned int max_sync_ms = 1000) {
		int fd;
		int ret = -1;
        stdin_unbuffered();
		this->key_thread = std::thread([&] { KeyEmulator::key_thread_emulator(); });
		for(int i= 0; i< 1000; i++) {
			  if((fd = open(getEvent().c_str(), O_RDONLY)) > 0) {
				  close(fd);
				  ret = 0;
				  break;
			  }
			  usleep(max_sync_ms); // 1ms
		}
		// fails if event not available in 1 second
		return(ret);
	}
#pragma GCC diagnostic pop
	/**
	 * Stop emulator
	 */
	__attribute__((deprecated)) void stop() {
		this->running = false;
		this->key_thread.join();
		stdin_buffered();
	}
	__attribute__((deprecated)) void join() {
		this->key_thread.join();
		stdin_buffered();
	}
	__attribute__((deprecated)) bool isRunning() const { return(running); }

	const string& getEvent() const {
		return(event);
	}

	const string& getInput() const {
		return(input);
	}

	__attribute__((deprecated)) unsigned int  getKeyUsAutoRelease() const {
		return key_us_auto_release;
	}

	__attribute__((deprecated)) void setKeyUsAutoRelease(unsigned int keyUsAutoRelease) {
		key_us_auto_release = keyUsAutoRelease;
	}




};


#endif /* KEYEMULATOR_HPP_ */
