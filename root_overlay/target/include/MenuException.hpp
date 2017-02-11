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

#ifndef HOMERD_INCLUDE_MENUEXCEPTION_HPP_
#define HOMERD_INCLUDE_MENUEXCEPTION_HPP_
namespace homerio {
class MenuException : public std::exception, public std::nested_exception {
 public:
  MenuException(const char* msg)
      : mMsg(msg) {
  }
  virtual ~MenuException() noexcept {
  }
  virtual const char* what() const noexcept {
    return mMsg.c_str();
  }
 protected:
  std::string mMsg;
};

class MenuEmptyException : public MenuException {
 public:
  MenuEmptyException(const char* msg)
      : MenuException(msg) {
  }
  virtual ~MenuEmptyException() noexcept {
  }
};
}

#endif /* HOMERD_INCLUDE_MENUEXCEPTION_HPP_ */
