#ifndef __ASE_READER_VERTEX_HPP__
#define __ASE_READER_VERTEX_HPP__

#include <iomanip>
#include <istream>
#include <sstream>

namespace ase_reader {

template <typename T>
class Vertex {
 public:
  Vertex() = default;

  Vertex(T x, T y, T z) : _x{x}, _y{y}, _z{z} {}

  auto x() const { return _x; }

  auto y() const { return _y; }

  auto z() const { return _z; }

  auto toString(int precision = 4) const -> std::string {
    std::stringstream ss;
    ss << std::setprecision(precision) << std::fixed;
    ss << x() << "\t" << y() << "\t" << z();
    return ss.str();
  }

 private:
  T _x{};
  T _y{};
  T _z{};
};

template <typename T>
auto &operator>>(std::istream &is, Vertex<T> &v) {
  using V = Vertex<T>;

  T x{};
  T y{};
  T z{};
  int vertex_index{};
  is >> vertex_index;
  is >> x >> y >> z;
  v = V{x, y, z};
  return is;
}

}  // namespace ase_reader

#endif  // __ASE_READER_VERTEX_HPP__
