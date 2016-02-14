/*
 * Observer.cpp
 *
 *  Created on: 14/feb/2016
 *      Author: daniele
 */



#include "Observer.h"

namespace obs {

namespace detail {

UniversalPtr createHeartBeat()
{
  return createEmptyPtr([] (void*) {});
}

}


}
