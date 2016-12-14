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

#ifndef MENUCOMPONENT_HPP_
#define MENUCOMPONENT_HPP_

#include <MenuAble.hpp>
#include <MenuComponent.hpp>
#include <MenuActionVisitors.hpp>
#include <MenuNavigatorVisitor.hpp>
#include <KeyPanel.hpp>
#include <memory>

using namespace std;

namespace homerio {

class MenuComponent {

 public:
  virtual ~MenuComponent() {
  }
  ;
  virtual MenuAble& getElement() = 0;
  // Visitors interface
  virtual void execEnter(MenuActionVisitor& m, KeyButton& k) = 0;
  virtual void execLeave(MenuActionVisitor& m, KeyButton& k) = 0;
  virtual void execClick(MenuActionVisitor& m, KeyButton& k) = 0;
  virtual shared_ptr<MenuComponent> execMove(MenuNavigatorVisitor& m,
                                             KeyButton& k) = 0;
};
}
#endif /* MENUCOMPONENT_HPP_ */
