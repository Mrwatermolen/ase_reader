#ifndef __ASE_READER_FACE_HPP__
#define __ASE_READER_FACE_HPP__

#include <sstream>

namespace ase_reader {

template <typename T>
class Face {
 public:
  static constexpr auto V1_HEADER = "A:";

  static constexpr auto V2_HEADER = "B:";

  static constexpr auto V3_HEADER = "C:";

  static constexpr auto INVALID_INDEX = static_cast<T>(-1);

  Face() = default;

  Face(T index, T v1, T v2, T v3) : _index(index), _v1(v1), _v2(v2), _v3(v3) {}

  auto index() const { return _index; }

  auto v1() const { return _v1; }

  auto v2() const { return _v2; }

  auto v3() const { return _v3; }

  auto toString() const {
    std::stringstream ss;
    ss << index() << "\t" << v1() << "\t" << v2() << "\t" << v3();
    return ss.str();
  }

 private:
  T _index{INVALID_INDEX};
  T _v1{INVALID_INDEX};
  T _v2{INVALID_INDEX};
  T _v3{INVALID_INDEX};
};

template <typename T>
auto &operator>>(std::istream &is, Face<T> &f) {
  std::string flag{};

  T index{};
  T v1{};
  T v2{};
  T v3{};

  is >> index;

  while (is >> flag) {
    if (flag == Face<T>::V1_HEADER) {
      is >> v1;
      continue;
    }

    if (flag == Face<T>::V2_HEADER) {
      is >> v2;
      continue;
    }

    if (flag == Face<T>::V3_HEADER) {
      is >> v3;
      continue;
    }
  }

  f = Face<T>{index, v1, v2, v3};

  return is;
}

}  // namespace ase_reader

#endif  // __ASE_READER_FACE_HPP__
