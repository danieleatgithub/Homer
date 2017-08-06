/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

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
  void refresh(chrono::system_clock::time_point time_point) {
  }
};

}
#endif /* SIMPLEMENUELEMENT_HPP_ */
