#include <Fakedisplay.h>
#include <stdio.h>
#include <cstdint>
#include <iostream>

using namespace std;

namespace homerio {

FakeDisplay::FakeDisplay(const char *bus) :
		Winstar(bus) {

}
FakeDisplay::FakeDisplay(const char *bus, const char *rst,
		const char *backlight) :
		Winstar(bus) {

}

FakeDisplay::~FakeDisplay() {
}

int FakeDisplay::dpy_open() {

	return (device_init());
}

int FakeDisplay::dpy_close() {

	return (0);
}

int FakeDisplay::dpy_write(int type, uint8_t data) {
	if (type == WSTAR_CMD) {
		printf("DEBUG i2cset -y 0x%x 0x%x 0x%x\n", address, type, data);
	} else {
		cout << data;
	}
	return (0);
}

}

