/*
 * Menu.hpp
 *
 * Reference:
 * 		http://stackoverflow.com/questions/13634189/best-way-to-construct-a-console-menu-class-hierachy
 * 		https://sourcemaking.com/design_patterns/composite
 * 		https://sourcemaking.com/design_patterns/visitor
 *
 *  Created on: 18/lug/2016
 *      Author: daniele
 */

#ifndef MENUABLE_HPP_
#define MENUABLE_HPP_

#include <string>
using namespace std;

namespace homerio {
class MenuAble {
 public:
  virtual ~MenuAble() {
  }
  ;
  virtual const string getLabel() const = 0;
  virtual const string getValue() const = 0;
  virtual void update() = 0;
};
}
#endif /* MENUABLE_HPP_ */
