/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MOVEVISITOR_HPP_
#define MOVEVISITOR_HPP_
#include <Menu.hpp>
#include <MenuException.hpp>
#include <MenuVisitors.hpp>
#include <stack>

namespace homerio {

class MoveVisitor : public MenuNavigatorVisitor {
  stack<reference_wrapper<SubMenu>> history;

 public:
  MoveVisitor(shared_ptr<SubMenu> m) {
    history.push(*m.get());
  }
  ;
  virtual ~MoveVisitor() {
  }
  ;
  /**
   * Reset history and jump back to home
   * @return
   * pointer to the first element in root menu
   */
  shared_ptr<MenuComponent> home() {
    while (history.size() > 1) {
      history.top().get().home();
      history.pop();
    }
    history.top().get().home();
    return (history.top().get().get_active_element());
  }

  virtual shared_ptr<MenuComponent> move(MenuLeaf& m, KeyButton& k) {
    switch (k.get_key()) {
      case BUTTON_UP:
        return (history.top().get().get_previous_element());
      case BUTTON_DOWN:
        return (history.top().get().get_next_element());
      case BUTTON_LEFT:
        if (history.size() > 1) {
          history.top().get().home();
          history.pop();
        }
        return (history.top().get().get_active_element());
      case BUTTON_RIGHT:
        return (history.top().get().get_active_element());
      default:
        throw MenuException((string("Unexpected key ") + string(k)).c_str());
        break;
    }
  }

  virtual shared_ptr<MenuComponent> move(SubMenu& m, KeyButton& k) {
    shared_ptr < MenuComponent > next_entry;
    switch (k.get_key()) {
      case BUTTON_UP:
        next_entry = history.top().get().get_previous_element();
        return (next_entry);
      case BUTTON_DOWN:
        next_entry = history.top().get().get_next_element();
        return (next_entry);
      case BUTTON_LEFT:
        if (history.size() > 1) {
          history.top().get().home();
          history.pop();
        }
        next_entry = history.top().get().get_active_element(); /* Raise a MenuEmptyException if next submenu is empty */
        return (next_entry);
      case BUTTON_RIGHT:
        history.push(m);  // Store current in history
        try {
          // try to enter in submenu
          next_entry = history.top().get().get_active_element(); /* Raise a MenuEmptyException if next submenu is empty */
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
};

}

#endif /* MOVEVISITOR_HPP_ */
