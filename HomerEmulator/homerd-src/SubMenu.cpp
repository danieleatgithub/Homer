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

#include <SubMenu.hpp>

using namespace std;

namespace homerio {

SubMenu::SubMenu(MenuAble& mable)
    : element(mable) {
  children.clear();
  cursor = children.begin();
}

/**
 *
 * @param c
 * MenuComponent to add to SubMenu
 */
void SubMenu::add(shared_ptr<MenuComponent> c) {
  children.push_back(c);
  cursor = children.begin();
}

/**
 *
 * @return
 * pointer to the active element in submenu
 */
shared_ptr<MenuComponent> SubMenu::getActiveElement() throw (MenuEmptyException) {
  if (children.empty())
    throw MenuEmptyException(
        (string("Empty Menu ") + element.getLabel()).c_str());
  return (*cursor);
}

/**
 * Go to the top of the menu
 * @return
 * pointer to the first element in submenu
 */
void SubMenu::home() {
  cursor = children.begin();
}

/**
 *
 * @return
 * pointer to the previous element in submenu
 */
shared_ptr<MenuComponent> SubMenu::getPreviousElement() {
  if (cursor != children.begin())
    cursor--;
  return (getActiveElement());
}
/**
 *
 * @return
 * pointer to the next element in submenu
 */
shared_ptr<MenuComponent> SubMenu::getNextElement() {
  cursor++;
  if (cursor == children.end())
    cursor--;
  return (getActiveElement());
}

MenuAble& SubMenu::getElement() {
  return element;
}
// Visitor Interfaces
void SubMenu::execClick(MenuActionVisitor& m, KeyButton& k) {
  m.click(*this, k);
}
void SubMenu::execEnter(MenuActionVisitor& m, KeyButton& k) {
  m.enter(*this, k);
}
void SubMenu::execLeave(MenuActionVisitor& m, KeyButton& k) {
  m.leave(*this, k);
}

shared_ptr<MenuComponent> SubMenu::execMove(MenuNavigatorVisitor& m,
                                            KeyButton& k) {
  return (m.move(*this, k));
}

const vector<shared_ptr<MenuComponent> >& SubMenu::getChildren() const {
  return children;
}

}

