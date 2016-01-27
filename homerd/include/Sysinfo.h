/*
 * Sysinfo.h
 *
 *  Created on: 27/gen/2016
 *      Author: daniele
 */

#ifndef SYSINFO_H_
#define SYSINFO_H_
#include <string>

using namespace std;

namespace homerio {

class Sysinfo {
private:
	Sysinfo() {};
	string ip;

public:

	static Sysinfo& get_instance() {
		static Sysinfo instance;
		return instance;
	}

	string get_local_ip(const char *ifname);
};

} /* namespace homerio */

#endif /* SYSINFO_H_ */
