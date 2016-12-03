/*
 * MenuException.hpp
 *
 *  Created on: 19/nov/2016
 *      Author: daniele
 */

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
