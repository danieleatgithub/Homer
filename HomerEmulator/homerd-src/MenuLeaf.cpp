/*
 * MenuLeaf.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

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

