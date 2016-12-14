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

#include <MenuDisplayVisitor.hpp>
#include <MenuLeaf.hpp>
#include <SubMenu.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <memory>

using namespace std;
using namespace log4cplus;

namespace homerio {

void MenuDisplayVisitor::enter(MenuLeaf& l, KeyButton& k) {
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.line2_home();
  dp.dpy_puts(l.getElement().getValue().c_str());
}
void MenuDisplayVisitor::leave(MenuLeaf& l, KeyButton& k) {
  dp.clear();
}
void MenuDisplayVisitor::click(MenuLeaf& l, KeyButton& k) {
  l.getElement().update();
  dp.clear();
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.line2_home();
  dp.dpy_puts(l.getElement().getValue().c_str());
}

void MenuDisplayVisitor::enter(SubMenu& l, KeyButton& k) {
  dp.dpy_puts(l.getElement().getLabel().c_str());
  dp.dpy_puts(">");
}
void MenuDisplayVisitor::leave(SubMenu& l, KeyButton& k) {
  dp.clear();
}
void MenuDisplayVisitor::click(SubMenu& l, KeyButton& k) {

}

}
