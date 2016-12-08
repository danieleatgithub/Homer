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
