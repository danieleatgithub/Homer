/*
 * Pin.h
 *
 *  Created on: 05/apr/2015
 *      Author: daniele
 */

#ifndef PIN_H_
#define PIN_H_
#include <string>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64


enum State_e {
	STATE_OFF = 0,
	STATE_ON,
	STATE_TOGGLE
};
enum Direction_e {
	OUT = 0,
	INP
};

enum Edge_e {
	RISING = 0,
	FALLING,
	NONE,
	BOTH
};


using namespace std;

class Pin {
private:
	string kstr;
	int kid;
	int fd;
	bool in_use;
	string name;
	Edge_e edge;
	Direction_e direction;
	bool last_value;
	void init();
	int write_value(bool value);
public:
	static struct pinmap_s* getPinDescriptor(const char *name);
	Pin(struct pinmap_s *pin_desc );
	Pin(int kid, string kstr, string name);
	virtual ~Pin();
	int get_pin();
	int pin_export();
	int pin_unexport();
	int set_direction(Direction_e dir);
	int set_edge(Edge_e edge);
	int setState(State_e state);
	int flip(unsigned int us);
	int get(bool *value);
	int pin_open();
	int pin_close();


};

#endif /* PIN_H_ */
