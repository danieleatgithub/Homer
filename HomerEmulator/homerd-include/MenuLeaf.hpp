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

#ifndef MENULEAF_HPP_
#define MENULEAF_HPP_

#include <memory>

#include <MenuComponent.hpp>
#include <MenuActionVisitors.hpp>
#include <MenuNavigatorVisitor.hpp>
#include <KeyPanel.hpp>

using namespace std;

namespace homerio {

/**
 * Terminal passive Menu element
 */
class MenuLeaf : public MenuComponent {
  MenuAble& element;

 public:
  MenuLeaf(MenuAble& mable)
      : element(mable) {
  }
  virtual MenuAble& getElement();
  // Visitor Interfaces
  virtual void execClick(MenuActionVisitor& m, KeyButton& k);
  virtual void execEnter(MenuActionVisitor& m, KeyButton& k);
  virtual void execLeave(MenuActionVisitor& m, KeyButton& k);
  virtual shared_ptr<MenuComponent> execMove(MenuNavigatorVisitor& m,
                                             KeyButton& k);

};
}
#endif /* MENULEAF_HPP_ */
