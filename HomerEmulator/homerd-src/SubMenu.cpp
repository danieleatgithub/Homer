/*
 * SubMenu.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

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

