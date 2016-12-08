/*
 * Menu.hpp
 *
 * Reference:
 * 		http://stackoverflow.com/questions/13634189/best-way-to-construct-a-console-menu-class-hierachy
 * 		https://sourcemaking.com/design_patterns/composite
 * 		https://sourcemaking.com/design_patterns/visitor
 *
 *  Created on: 18/lug/2016
 *      Author: daniele
 */

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
