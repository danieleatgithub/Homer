/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MENUNAVIGATORVISITOR_HPP_
#define MENUNAVIGATORVISITOR_HPP_

#include <memory>

using namespace std;

namespace homerio {

// circular dependency
class MenuComponent;
class MenuLeaf;
class SubMenu;

class MenuNavigatorVisitor {
 public:
  virtual ~MenuNavigatorVisitor() {
  }
  ;
  virtual shared_ptr<MenuComponent> move(MenuLeaf& m, KeyButton& k) = 0;
  virtual shared_ptr<MenuComponent> move(SubMenu& m, KeyButton& k) = 0;

};
}

#endif /* MENUNAVIGATORVISITOR_HPP_ */
