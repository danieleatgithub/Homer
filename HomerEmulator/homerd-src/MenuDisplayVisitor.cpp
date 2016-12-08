/*
 * MenuDisplayVisitor.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

#include <MenuDisplayVisitor.hpp>
#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <memory>

using namespace std;
using namespace log4cplus;

namespace homerio {

void MenuDisplayVisitor::enter(MenuLeaf& l, KeyButton& k) {
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.line2_home();
  dp.dpy_puts(l.getElement().getValue().c_str());
}
void MenuDisplayVisitor::leave(MenuLeaf& l, KeyButton& k) {
  dp.clear();
}
void MenuDisplayVisitor::click(MenuLeaf& l, KeyButton& k) {
  l.getElement().update();
  dp.clear();
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.line2_home();
  dp.dpy_puts(l.getElement().getValue().c_str());
}

void MenuDisplayVisitor::enter(SubMenu& l, KeyButton& k) {
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.dpy_puts(">");
}
void MenuDisplayVisitor::leave(SubMenu& l, KeyButton& k) {
  dp.clear();
}
void MenuDisplayVisitor::click(SubMenu& l, KeyButton& k) {

}

}
