/*
 * DisplayVisitor.hpp
 *
 *  Created on: 16/ago/2016
 *      Author: daniele
 */

#ifndef MOVEVISITOR_HPP_
#define MOVEVISITOR_HPP_

#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <MenuException.hpp>
#include <stack>

namespace homerio {

class MenuMoveVisitor : public MenuNavigatorVisitor {
  stack<reference_wrapper<SubMenu>> history;

 public:
  MenuMoveVisitor(shared_ptr<SubMenu> m);

  virtual ~MenuMoveVisitor() {
  }
  ;
  /**
   * Reset history and jump back to home
   * @return
   * pointer to the first element in root menu
   */
  shared_ptr<MenuComponent> home();
  virtual shared_ptr<MenuComponent> move(MenuLeaf& m, KeyButton& k);
  virtual shared_ptr<MenuComponent> move(SubMenu& m, KeyButton& k);
};

}

#endif /* MOVEVISITOR_HPP_ */
