/*
 * HwEmulated.cpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */


#include <Winstar.h>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <HwAcquaA5.hpp>
#include <stdio.h>
#include <IDGenerator.h>
#include <fcntl.h>

namespace homerio {



int I2cBusAcquaA5::open(const char *file, int flag) {
	return(::open(file,flag));
}
int I2cBusAcquaA5::close(int fd) {
	return(::close(fd));
}
int I2cBusAcquaA5::read(int fd, void *buf, size_t nbyte) {
	return(::read(fd,buf,nbyte));
}
int I2cBusAcquaA5::write(int fd, const void *buffer, size_t size) {
	return(::write(fd,buffer,size));
}
int I2cBusAcquaA5::ioctl(int fd, unsigned long int request, ...) {
	va_list arguments;
	va_start ( arguments, request );
	switch(request) {
		case I2C_SLAVE:
			return(::ioctl(fd,request,va_arg(arguments,unsigned int)));
	}
	return(-1);
}

__off_t I2cBusAcquaA5::lseek (int fd, __off_t __offset, int __whence) {
	return(::lseek(fd,__offset,__whence));
}



int GpioPortAcquaA5::open(const char *file, int flag) {
	return(::open(file,flag));
}
int GpioPortAcquaA5::close(int fd) {
	return(::close(fd));
}
int GpioPortAcquaA5::read(int fd, void *buf, size_t nbyte) {
	return(::read(fd,buf,nbyte));
}
int GpioPortAcquaA5::write(int fd, const void *buffer, size_t size) {
	return(::write(fd,buffer,size));
}
int GpioPortAcquaA5::ioctl(int fd, unsigned long int request, ...) {
	return(-1);
}
__off_t GpioPortAcquaA5::lseek (int fd, __off_t __offset, int __whence) {
	return(::lseek(fd,__offset,__whence));
}

}
