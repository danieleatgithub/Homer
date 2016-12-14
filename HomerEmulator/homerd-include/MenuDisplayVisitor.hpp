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

#ifndef DISPLAYVISITOR_HPP_
#define DISPLAYVISITOR_HPP_

#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <MenuActionVisitors.hpp>
#include <Display.h>

namespace homerio {

class MenuDisplayVisitor : public MenuActionVisitor {
  Display& dp;
 public:
  MenuDisplayVisitor(Display& display)
      : dp(display) {
  }
  ;
  virtual ~MenuDisplayVisitor() {
  }
  ;

  virtual void enter(MenuLeaf& l, KeyButton& k);
  virtual void leave(MenuLeaf& l, KeyButton& k);
  virtual void click(MenuLeaf& l, KeyButton& k);
  virtual void enter(SubMenu& l, KeyButton& k);
  virtual void leave(SubMenu& l, KeyButton& k);
  virtual void click(SubMenu& l, KeyButton& k);

};

}

#endif /* DISPLAYVISITOR_HPP_ */
