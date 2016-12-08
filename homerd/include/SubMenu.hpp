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

#ifndef SUBMENU_HPP_
#define SUBMENU_HPP_

#include <iostream>
#include <vector>
#include <iterator>
#include <stack>
#include <memory>
#include <Scheduler.hpp>
#include <KeyPanel.hpp>

#include <MenuComponent.hpp>
#include <MenuException.hpp>
#include <MenuActionVisitors.hpp>
#include <MenuNavigatorVisitor.hpp>

using namespace std;

namespace homerio {

/**
 *
 * SubMenu component.
 * container of MenuComponents
 *
 */
class SubMenu : public MenuComponent {

 protected:
  vector<shared_ptr<MenuComponent>> children;
  vector<shared_ptr<MenuComponent>>::iterator cursor;
  MenuAble& element;

 public:
  SubMenu(MenuAble& mable);

  /**
   *
   * @param c
   * MenuComponent to add to SubMenu
   */
  void add(shared_ptr<MenuComponent> c);

  /**
   *
   * @return
   * pointer to the active element in submenu
   */
  shared_ptr<MenuComponent> getActiveElement() throw (MenuEmptyException);

  /**
   * Go to the top of the menu
   * @return
   * pointer to the first element in submenu
   */
  void home();

  /**
   *
   * @return
   * pointer to the previous element in submenu
   */
  shared_ptr<MenuComponent> getPreviousElement();
  /**
   *
   * @return
   * pointer to the next element in submenu
   */
  shared_ptr<MenuComponent> getNextElement();

  virtual MenuAble& getElement();
  // Visitor Interfaces
  virtual void execClick(MenuActionVisitor& m, KeyButton& k);
  virtual void execEnter(MenuActionVisitor& m, KeyButton& k);
  virtual void execLeave(MenuActionVisitor& m, KeyButton& k);

  virtual shared_ptr<MenuComponent> execMove(MenuNavigatorVisitor& m,
                                             KeyButton& k);

  const vector<shared_ptr<MenuComponent> >& getChildren() const;
};

}
#endif /* SUBMENU_HPP_ */
