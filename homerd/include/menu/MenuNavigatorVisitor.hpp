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

#ifndef MENUNAVIGATORVISITOR_HPP_
#define MENUNAVIGATORVISITOR_HPP_

#include <memory>

using namespace std;

namespace homerio {

// circular dependency
class MenuComponent;
class MenuLeaf;
class SubMenu;

class MenuNavigatorVisitor {
 public:
  virtual ~MenuNavigatorVisitor() {
  }
  ;
  virtual shared_ptr<MenuComponent> move(MenuLeaf& m, KeyButton& k) = 0;
  virtual shared_ptr<MenuComponent> move(SubMenu& m, KeyButton& k) = 0;

};
}

#endif /* MENUNAVIGATORVISITOR_HPP_ */
