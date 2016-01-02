/*
 * Pin.cpp
 *
 *  Created on: 05/apr/2015
 *      Author: daniele
 */

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "Pin.h"

const char *Direction_s[2] =	{ "out", "in"};
const char *Edge_s[4] =			{ "rising", "falling", "none", "both"};

using namespace std;

static struct pinmap_s {
	unsigned int kernel_id;
	const char *kernel_string;
	const char *name;
} pinmap[] ={
		0,	"pioA0"	,"PA00",
		1,	"pioA1"	,"PA01",
		2,	"pioA2"	,"PA02",
		3,	"pioA3"	,"PA03",
		77,	"pioC13","PC13"
};

Pin::~Pin() {
	// TODO Auto-generated destructor stub
}
void Pin::init() {
	this->kstr = "";
	this->kid = -1;
	this->name = "";
	this->fd = -1;
	this->direction = INP;
	this->edge = RISING;
	this->in_use = false;
	this->last_value = false;
}
Pin::Pin(struct pinmap_s *pin_desc ){
	init();
	if(pin_desc) {
		this->kstr 	= pin_desc->kernel_string;
		this->kid 	= pin_desc->kernel_id;
		this->name 	= pin_desc->name;
	}
}

Pin::Pin(int kid, string kstr, string name) {
	init();
	this->kstr = kstr;
	this->kid = kid;
	this->name = name;
}

struct pinmap_s *Pin::getPinDescriptor(const char *name) {
	unsigned int i=0;
	for(i=0;i<(sizeof(pinmap)/sizeof(struct pinmap_s));i++) {
		if(strcmp(pinmap[i].name,name) == 0) {
			return(&pinmap[i]);
		}
	}
	return(NULL);
}

int Pin::get_pin() {
	return(this->kid);
}

int Pin::pin_export() {
	int fd, len;
	char buf[MAX_BUF];
	if(kid < 0) return(-1);
	if(this->in_use) {
		perror("Already in use");
		return -2;
	}
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
	len = snprintf(buf, sizeof(buf), "%d", this->kid);
	printf("DEBUG echo %s/export %s\n",SYSFS_GPIO_DIR,buf);
	write(fd, buf, len);
	close(fd);
	this->in_use = true;
	return 0;

}
int Pin::pin_unexport() {
	int fd, len;
	char buf[MAX_BUF];
	if(kid < 0) return(-1);
	if(!this->in_use) {
		perror("Not in use");
		return -100;
	}
	if(this->fd > 0) {
		close(this->fd);
	}
	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", this->kid);
	printf("DEBUG echo %s/unexport %s\n",SYSFS_GPIO_DIR,buf);
	write(fd, buf, len);
	close(fd);
	this->in_use = false;
	return 0;

}
int Pin::set_direction(Direction_e dir)
{
	int fd, ret;
	char buf[MAX_BUF];
	if(kid < 0) return(-1);
	if(!this->in_use) {
		perror("Not in use");
		return -100;
	}

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/%s/direction", this->kstr.c_str());

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}
	printf("DEBUG echo %s %s\n",buf,Direction_s[dir]);
	ret = write(fd, Direction_s[dir], (strlen(Direction_s[dir])+1));
	close(fd);
	if(ret < 0 ) {
		return(ret);
	}
	this->direction = dir;
	// Default out value is low
	if(this->direction == OUT) this->last_value = false;
	return 0;
}
int Pin::set_edge(Edge_e edge)
{
	int fd, ret;
	char buf[MAX_BUF];
	if(kid < 0) return(-1);
	if(!this->in_use) {
		perror("Not in use");
		return -100;
	}

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/edge", this->kstr.c_str());

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}
	printf("DEBUG echo %s/%s/edge %s\n",SYSFS_GPIO_DIR,this->kstr.c_str(),Edge_s[edge]);
	ret = write(fd, Edge_s[edge], strlen(Edge_s[edge]) + 1);
	close(fd);
	if(ret < 0 ) {
		return(ret);
	}
	return 0;
}

int Pin::setState(State_e state) {
	bool newval;
	if(kid < 0) return(-1);
	if(state == STATE_OFF || (last_value  && state == STATE_TOGGLE)) newval = false;
	if(state == STATE_ON  || (!last_value && state == STATE_TOGGLE)) newval = true;
	if(write_value(newval) < 0) return(-1);
	last_value = newval;
	return(0);
}
int Pin::flip(unsigned int us) {
	if(kid < 0) return(-1);
	setState(STATE_TOGGLE);
	usleep(us);
	return(setState(STATE_TOGGLE));
}
int Pin::write_value(bool value)
{
	int fd;
	char buf[MAX_BUF];
	char *set="1";
	char *unset="0";
	char *val=unset;

	if (value) 	val = set;
	printf("DEBUG %s/%s/value %s\n",SYSFS_GPIO_DIR,this->kstr.c_str(),val);

	if(kid < 0) return(-1);
	if(!this->in_use || this->direction != OUT) {
		perror("Not in use or input pin");
		return -100;
	}
	if(this->fd <= 0) {
		snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value", this->kstr.c_str());

		fd = open(buf, O_WRONLY);
		if (fd < 0) {
			perror("gpio/set-value");
			return fd;
		}
		write(fd, val, 2);
		close(fd);
	} else {
		write(this->fd, val, 2);
		lseek(this->fd, 0, SEEK_SET);
	}
	this->last_value = value;
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int Pin::get(bool *value)
{
	int fd;
	char buf[MAX_BUF];
	char ch;

	if(kid < 0) return(-1);
	//len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value", this->kstr.c_str());

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = true;
	} else {
		*value = false;
	}

	close(fd);
	return 0;
}
int Pin::pin_open()
{
	char buf[MAX_BUF];

	if(kid < 0) return(-1);
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value", this->kstr.c_str());

	if(this->direction == OUT )	this->fd = open(buf, O_WRONLY | O_NONBLOCK );
	else 						this->fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return this->fd;
}



int Pin::pin_close()
{
	if(kid < 0) return(-1);
	return close(this->fd);
}
