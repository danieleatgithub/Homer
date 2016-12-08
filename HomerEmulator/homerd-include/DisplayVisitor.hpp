/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <Menu.hpp>
#include <MenuVisitors.hpp>
#include <Display.h>

namespace homerio {

class DisplayVisitor : public MenuActionVisitor {
  Display& dp;
 public:
  DisplayVisitor(Display& display)
      : dp(display) {
  }
  ;
  virtual ~DisplayVisitor() {
  }
  ;

  virtual void enter(MenuLeaf& l, KeyButton& k) {
    dp.dpy_puts(l.getElement().getLabel().c_str());
    dp.line2_home();
    dp.dpy_puts(l.getElement().getValue().c_str());
  }
  virtual void leave(MenuLeaf& l, KeyButton& k) {
    dp.clear();
  }
  virtual void click(MenuLeaf& l, KeyButton& k) {
    l.getElement().update();
    dp.clear();
    dp.dpy_puts(l.getElement().getLabel().c_str());
    dp.line2_home();
    dp.dpy_puts(l.getElement().getValue().c_str());
  }

  virtual void enter(SubMenu& l, KeyButton& k) {
    dp.dpy_puts(l.getElement().getLabel().c_str());
    dp.dpy_puts(">");
  }
  virtual void leave(SubMenu& l, KeyButton& k) {
    dp.clear();
  }
  virtual void click(SubMenu& l, KeyButton& k) {

  }

};

}

#endif /* DISPLAYVISITOR_HPP_ */
