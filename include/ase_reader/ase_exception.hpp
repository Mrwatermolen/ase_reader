#ifndef __ASE_READER_ASE_EXCEPTION_HPP__
#define __ASE_READER_ASE_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <string_view>

namespace ase_reader {

class ASEException : public std::exception {
 public:
  explicit ASEException(std::string_view msg) : _msg{msg} {}

  const char *what() const noexcept override { return _msg.c_str(); }

 private:
  std::string _msg;
};
}  // namespace ase_reader

#endif  // __ASE_READER_ASE_EXCEPTION_HPP__
