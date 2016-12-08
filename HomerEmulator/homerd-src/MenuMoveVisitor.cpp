/*
 * MenuMoveVisitor.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

#include <MenuMoveVisitor.hpp>
#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <memory>

using namespace std;
using namespace log4cplus;

namespace homerio {

MenuMoveVisitor::MenuMoveVisitor(shared_ptr<SubMenu> m) {
  history.push(*m.get());
}

shared_ptr<MenuComponent> MenuMoveVisitor::home() {
  while (history.size() > 1) {
    history.top().get().home();
    history.pop();
  }
  history.top().get().home();
  return (history.top().get().getActiveElement());
}

shared_ptr<MenuComponent> MenuMoveVisitor::move(MenuLeaf& m, KeyButton& k) {
  switch (k.getKey()) {
    case BUTTON_UP:
      return (history.top().get().getPreviousElement());
    case BUTTON_DOWN:
      return (history.top().get().getNextElement());
    case BUTTON_LEFT:
      if (history.size() > 1) {
        history.top().get().home();
        history.pop();
      }
      return (history.top().get().getActiveElement());
    case BUTTON_RIGHT:
      return (history.top().get().getActiveElement());
    default:
      throw MenuException((string("Unexpected key ") + string(k)).c_str());
      break;
  }
}

shared_ptr<MenuComponent> MenuMoveVisitor::move(SubMenu& m, KeyButton& k) {
  shared_ptr < MenuComponent > next_entry;
  switch (k.getKey()) {
    case BUTTON_UP:
      next_entry = history.top().get().getPreviousElement();
      return (next_entry);
    case BUTTON_DOWN:
      next_entry = history.top().get().getNextElement();
      return (next_entry);
    case BUTTON_LEFT:
      if (history.size() > 1) {
        history.top().get().home();
        history.pop();
      }
      next_entry = history.top().get().getActiveElement(); /* Raise a MenuEmptyException if next submenu is empty */
      return (next_entry);
    case BUTTON_RIGHT:
      history.push(m);  // Store current in history
      try {
        // try to enter in submenu
        next_entry = history.top().get().getActiveElement(); /* Raise a MenuEmptyException if next submenu is empty */
      } catch (MenuEmptyException& e) {
        // empty menu pop from history and forward exception
        history.pop();
        throw e;
      }
      return (next_entry);
    default:
      throw MenuException((string("Unexpected key ") + string(k)).c_str());
      break;
  }
}

}
