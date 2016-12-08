/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <MenuActionVisitors.hpp>
#include <Display.h>

namespace homerio {

class MenuDisplayVisitor : public MenuActionVisitor {
  Display& dp;
 public:
  MenuDisplayVisitor(Display& display)
      : dp(display) {
  }
  ;
  virtual ~MenuDisplayVisitor() {
  }
  ;

  virtual void enter(MenuLeaf& l, KeyButton& k);
  virtual void leave(MenuLeaf& l, KeyButton& k);
  virtual void click(MenuLeaf& l, KeyButton& k);
  virtual void enter(SubMenu& l, KeyButton& k);
  virtual void leave(SubMenu& l, KeyButton& k);
  virtual void click(SubMenu& l, KeyButton& k);

};

}

#endif /* DISPLAYVISITOR_HPP_ */
