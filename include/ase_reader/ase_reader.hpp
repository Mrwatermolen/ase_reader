#ifndef __ASE_READER_ASE_READER_HPP__
#define __ASE_READER_ASE_READER_HPP__

#include <ase_reader/ase_exception.hpp>
#include <ase_reader/ase_object.hpp>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string_view>
#include <vector>

namespace ase_reader {

class ASEReaderException : public ASEException {
 public:
  using ASEException::ASEException;
};

template <typename T = float>
class ASEReader {
 public:
  using ASEObject = ASEObject<T>;
  using ASEMesh = typename ASEObject::ASEMesh;

 public:
  auto precision() const { return _precision; }

  auto setPrecision(int precision) {
    _precision = precision;
    for (auto &&object : _objects) {
      object.setPrecision(precision);
    }
  }

  auto &objects() const { return _objects; }

  auto &objects() { return _objects; }

  auto read(std::string_view filename) -> void {
    std::ifstream file{filename.data()};
    if (!file.is_open()) {
      throw ASEReaderException{"Failed to open file"};
    }

    read(file);
  }

  auto read(std::ifstream &is) -> void { _objects = readOject(is); }

  auto save(std::string_view dir) const -> void {
    std::filesystem::path dir_path{dir};
    if (_objects.empty()) {
      throw ASEReaderException{"save: No objects to save"};
    }

    if (!std::filesystem::exists(dir_path)) {
      std::filesystem::create_directories(dir_path);
      std::cout << "INFO: ase_reader::ASEReader::save: Created directory: "
                << dir << '\n';
    }

    auto info_os =
        std::ofstream{(dir_path / "info.txt").string(), std::ios::out};
    auto vertices_os =
        std::ofstream{(dir_path / "vertices.txt").string(), std::ios::out};
    auto elements_os =
        std::ofstream{(dir_path / "elements.txt").string(), std::ios::out};

    save(info_os, vertices_os, elements_os);
  }

  auto save(std::ostream &info_os, std::ostream &vertices_os,
            std::ostream &elements_os) const -> void {
    std::size_t num_vertices = 0;
    std::size_t num_faces = 0;

    for (const auto &object : _objects) {
      std::stringstream obj_info_os;
      std::stringstream obj_vertices_os;
      std::stringstream obj_elements_os;
      object.save(obj_info_os, obj_vertices_os, obj_elements_os);

      vertices_os << obj_vertices_os.rdbuf();

      while (obj_elements_os.good()) {
        std::size_t v1{};
        std::size_t v2{};
        std::size_t v3{};
        obj_elements_os >> v1 >> v2 >> v3;
        if (obj_elements_os.good()) {
          elements_os << v1 + num_vertices << ' ' << v2 + num_vertices << ' '
                      << v3 + num_vertices << '\n';
        }
      }

      std::size_t num_vertices_obj = 0;
      std::size_t num_faces_obj = 0;
      obj_info_os >> num_vertices_obj;
      num_vertices += num_vertices_obj;
      obj_info_os >> num_faces_obj;
      num_faces += num_faces_obj;
    }

    info_os << num_vertices << '\n';
    info_os << num_faces << '\n';
  }

 private:
  static auto moveToHeader(__attribute__((unused)) std::ifstream &is) -> void {
  };

  static auto readOject(std::ifstream &is) -> std::vector<ASEObject> {
    moveToHeader(is);
    auto objects = std::vector<ASEObject>{};

    try {
      while (!is.eof()) {
        auto object = ASEObject{};
        object.read(is);
        objects.emplace_back(object);
      }
    } catch (const ASEObjectException &e) {
      if (!objects.empty()) {
        return objects;
      }
      std::stringstream ss;
      ss << "readOject: Failed to read object. Error: " << e.what();
      throw ASEReaderException{ss.str()};
    }

    if (objects.empty()) {
      throw ASEReaderException{"readOject: No objects found"};
    }

    return objects;
  }

 private:
  int _precision{4};
  std::vector<ASEObject> _objects{};
};

}  // namespace ase_reader

#endif  // __ASE_READER_ASE_READER_HPP__
