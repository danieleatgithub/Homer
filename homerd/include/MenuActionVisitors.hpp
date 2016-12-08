/*
 * MenuComponentVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MENUACTIONVISITORS_HPP_
#define MENUACTIONVISITORS_HPP_

#include <KeyPanel.hpp>

namespace homerio {

// circular dependency
class MenuLeaf;
class SubMenu;

class MenuActionVisitor {
 public:
  virtual ~MenuActionVisitor() {
  }
  ;
  virtual void enter(MenuLeaf& l, KeyButton& k) = 0;
  virtual void leave(MenuLeaf& l, KeyButton& k) = 0;
  virtual void click(MenuLeaf& l, KeyButton& k) = 0;

  virtual void enter(SubMenu& l, KeyButton& k) = 0;
  virtual void leave(SubMenu& l, KeyButton& k) = 0;
  virtual void click(SubMenu& l, KeyButton& k) = 0;

};

}

#endif /* MENUVISITORS_HPP_ */
