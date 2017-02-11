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
