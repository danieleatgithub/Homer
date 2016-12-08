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

#ifndef SIMPLEMENUELEMENT_HPP_
#define SIMPLEMENUELEMENT_HPP_

#include <MenuAble.hpp>

using namespace std;

namespace homerio {

class SimpleMenuElement : public MenuAble {
 private:
  string label;
  string value;
 public:
  SimpleMenuElement(string _label) {
    this->label = _label;
    this->value = "";
  }
  SimpleMenuElement(string _label, string _value) {
    this->label = _label;
    this->value = _value;
  }
  ~SimpleMenuElement() {
  }

  const string getLabel() const {
    return label;
  }
  const string getValue() const {
    return value;
  }

  void setLabel(const string& label) {
    this->label = label;
  }

  void setValue(const string& value) {
    this->value = value;
  }
  void update() {
    return;
  }
};

}
#endif /* SIMPLEMENUELEMENT_HPP_ */
