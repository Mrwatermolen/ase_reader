#ifndef __ASE_READER_ASE_OBJECT_HPP__
#define __ASE_READER_ASE_OBJECT_HPP__

#include <ase_reader/ase_exception.hpp>
#include <ase_reader/ase_mesh.hpp>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string_view>

namespace ase_reader {

class ASEObjectException : public ASEException {
 public:
  using ASEException::ASEException;
};

template <typename T = float>
class ASEObject {
 public:
  using ASEMesh = ASEMesh<T>;

  constexpr static auto HEADER = "*GEOMOBJECT";

  constexpr static auto NODE_NAME_HEADER = "*NODE_NAME";

 public:
  auto precision() const { return _mesh.precision(); }

  auto setPrecision(int precision) { _mesh.setPrecision(precision); }

  auto name() const { return _name; }

  auto &mesh() const { return _mesh; }

  auto &mesh() { return _mesh; }

  auto read(std::string_view filename) -> void {
    std::ifstream file{filename.data()};
    if (!file.is_open()) {
      throw ASEObjectException{"Failed to open file"};
    }

    read(file);
  }

  auto read(std::ifstream &is) -> void {
    moveToHeader(is);
    _name = readName(is);
    _mesh = readMesh(is);
  }

  auto saveToDir(std::string_view dir,
                 ASEMesh::FileMode mode = ASEMesh::FileMode::OVERWRITE,
                 std::size_t offset = 0) const -> void {
    _mesh.saveToDir(dir, mode, offset);
  }

  auto save(std::ostream &info_os, std::ostream &vertices_os,
            std::ostream &elements_os) const -> void {
    _mesh.save(info_os, vertices_os, elements_os);
  }

 private:
  std::string _name;
  ASEMesh _mesh{};

 private:
  static auto moveToHeader(std::ifstream &is) -> void {
    std::string line;
    while (std::getline(is, line)) {
      if (line.find(HEADER) != std::string::npos) {
        return;
      }
    }

    std::stringstream ss;
    ss << "moveToHeader: Failed to find " << HEADER
       << " header. Invalid file format.";
    throw ASEObjectException{ss.str()};
  }

  static auto readName(std::ifstream &is) -> std::string {
    std::string name;
    std::string line;
    while (std::getline(is, line)) {
      if (line.find(NODE_NAME_HEADER) != std::string::npos) {
        name = line.substr(line.find_first_of('\"') + 1);
        name = name.substr(0, name.find_last_of('\"'));

        return name;
      }
    }

    std::stringstream ss;
    ss << "readName: Failed to find " << NODE_NAME_HEADER
       << " header. Invalid file format.";
    throw ASEObjectException{ss.str()};
  }

  static auto readMesh(std::ifstream &is) -> ASEMesh {
    ASEMesh mesh;
    mesh.read(is);
    return mesh;
  }
};

}  // namespace ase_reader

#endif  // __ASE_READER_ASE_OBJECT_HPP__
