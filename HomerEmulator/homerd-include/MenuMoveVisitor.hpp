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
