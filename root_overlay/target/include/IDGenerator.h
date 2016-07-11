/*
 * IDGenerator.h
 *
 *  Created on: 01/apr/2016
 *      Author: daniele
 */

#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_
#include <cstdint>
#include <iostream>
namespace shd {

class IDGenerator {
public:
	static IDGenerator& get_istance() {
		static IDGenerator istance;
		return istance;
	}
	uint32_t getId() {
		return id++;
	}

private:
	IDGenerator () { id = 0; }
	uint32_t id;
};

} /* namespace shd */

#endif /* IDGENERATOR_H_ */
