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

#include <MenuLeaf.hpp>

using namespace std;

namespace homerio {

MenuAble& MenuLeaf::getElement() {
  return element;
}
// Visitor Interfaces
void MenuLeaf::execClick(MenuActionVisitor& m, KeyButton& k) {
  m.click(*this, k);
}
void MenuLeaf::execEnter(MenuActionVisitor& m, KeyButton& k) {
  m.enter(*this, k);
}
void MenuLeaf::execLeave(MenuActionVisitor& m, KeyButton& k) {
  m.leave(*this, k);
}
shared_ptr<MenuComponent> MenuLeaf::execMove(MenuNavigatorVisitor& m,
                                             KeyButton& k) {
  return (m.move(*this, k));
}

}

