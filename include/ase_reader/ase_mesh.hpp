#ifndef __ASE_READER_ASE_MESH_HPP__
#define __ASE_READER_ASE_MESH_HPP__

#include <ase_reader/ase_exception.hpp>
#include <ase_reader/face.hpp>
#include <ase_reader/vertex.hpp>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace ase_reader {

class ASEMeshException : public ASEException {
 public:
  using ASEException::ASEException;
};

template <typename Index = std::size_t, typename T = float>
class ASEMesh {
 public:
  using Vertex = Vertex<T>;
  using Face = Face<Index>;

  static constexpr auto MESH_HEADER = std::string_view{"*MESH"};
  static constexpr auto MESH_NUMVERTEX_HEADER =
      std::string_view{"*MESH_NUMVERTEX"};
  static constexpr auto MESH_NUMFACES_HEADER =
      std::string_view{"*MESH_NUMFACES"};
  static constexpr auto MESH_VERTEX_HEADER = std::string_view{"*MESH_VERTEX"};
  static constexpr auto MESH_FACE_HEADER = std::string_view{"*MESH_FACE"};
  static constexpr auto MESH_VERTEX_LIST_HEADER =
      std::string_view{"*MESH_VERTEX_LIST"};
  static constexpr auto MESH_FACE_LIST_HEADER =
      std::string_view{"*MESH_FACE_LIST"};

  enum FileMode { OVERWRITE, APPEND };

 public:
  auto numVertices() const { return _num_vertices; }

  auto numFaces() const { return _num_faces; }

  auto &vertices() const { return _vertices; }

  auto &faces() const { return _faces; }

  auto &vertices() { return _vertices; }

  auto &faces() { return _faces; }

  auto precision() const { return _precision; }

  auto setPrecision(int precision) { _precision = precision; }

  auto read(std::string_view filename) -> void {
    std::ifstream is(filename.data());

    if (!is.is_open()) {
      std::stringstream ss;
      ss << "readMesh: ";
      ss << "Cannot open file: " << filename;
      throw ASEMeshException(ss.str());
    }

    read(is);
  }

  auto read(std::ifstream &is) -> void {
    moveToMeshHeader(is);

    if (is.eof()) {
      std::stringstream ss;
      ss << "readMesh: ";
      ss << "Cannot find header: " << MESH_HEADER;
      throw ASEMeshException(ss.str());
    }

    _num_vertices = readMeshNumVertices(is);

    _num_faces = readMeshNumFaces(is);

    _vertices = readMeshVerticesList(is);

    _faces = readMeshFaceList(is);

    if (_vertices.size() != _num_vertices) {
      std::stringstream ss;
      ss << "readMesh: ";
      ss << "Information mismatch: ";
      ss << "Num Vertex: " << _num_vertices << " != ";
      ss << "Vertex Size: " << _vertices.size();
      throw ASEMeshException(ss.str());
    }

    if (_faces.size() != _num_faces) {
      std::stringstream ss;
      ss << "readMesh: ";
      ss << "Information mismatch: ";
      ss << "Num Faces: " << _num_faces << " != ";
      ss << "Faces Size: " << _faces.size();
      throw ASEMeshException(ss.str());
    }
  }

  auto save(std::ostream &info_os, std::ostream &vertices_os,
            std::ostream &elements_os) const -> void {
    saveInfo(info_os, _num_vertices, _num_faces);
    saveElements(elements_os, _faces);
    vertices_os << std::setprecision(_precision) << std::fixed;
    saveVertices(vertices_os, _vertices);
  }

 private:
  int _precision{4};

  std::size_t _num_vertices{};
  std::size_t _num_faces{};
  std::vector<Vertex> _vertices{};
  std::vector<Face> _faces{};

 private:
  static auto moveToMeshHeader(std::ifstream &is) -> void {
    std::string line;

    while (std::getline(is, line)) {
      if (line.find(MESH_HEADER) == std::string::npos) {
        continue;
      }

      break;
    }
  }

  static auto readMeshNumVertices(std::ifstream &is) -> std::size_t {
    std::string line;

    while (std::getline(is, line)) {
      if (line.find(MESH_NUMVERTEX_HEADER) == std::string::npos) {
        continue;
      }

      std::stringstream ss;
      ss << line;
      std::size_t value;
      std::string name;
      ss >> name >> value;
      if (name == MESH_NUMVERTEX_HEADER) {
        return value;
      }
    }

    std::stringstream ss;
    ss << "readMeshNumVertex: ";
    ss << "Can't find header: " << MESH_NUMVERTEX_HEADER;
    throw ASEMeshException(ss.str());
  }

  static auto readMeshNumFaces(std::ifstream &is) -> std::size_t {
    std::string line;

    while (std::getline(is, line)) {
      if (line.find(MESH_NUMFACES_HEADER) == std::string::npos) {
        continue;
      }

      std::stringstream ss;
      ss << line;
      std::size_t value;
      std::string name;
      ss >> name >> value;
      if (name == MESH_NUMFACES_HEADER) {
        return value;
      }
    }

    std::stringstream ss;
    ss << "readMeshNumFaces: ";
    ss << "Can't find header: " << MESH_NUMFACES_HEADER;
    throw ASEMeshException(ss.str());
  }

  static auto readMeshVerticesList(std::ifstream &is) -> std::vector<Vertex> {
    std::string line;

    while (std::getline(is, line)) {
      if (line.find(MESH_VERTEX_LIST_HEADER) != std::string::npos) {
        break;
      }
    }

    if (is.eof()) {
      std::stringstream ss;
      ss << "readMeshVertexList: ";
      ss << "Can't find header: " << MESH_VERTEX_LIST_HEADER;
    }

    std::vector<Vertex> vertex{};

    while (std::getline(is, line)) {
      std::stringstream ss;
      ss << line;

      std::string name;
      ss >> name;
      if (name != MESH_VERTEX_HEADER) {
        break;
      }

      Vertex v;
      ss >> v;

      vertex.emplace_back(v);
    }

    return vertex;
  }

  static auto readMeshFaceList(std::ifstream &is) -> std::vector<Face> {
    std::string line;
    while (std::getline(is, line)) {
      if (line.find(MESH_FACE_LIST_HEADER) != std::string::npos) {
        break;
      }
    }

    if (is.eof()) {
      std::stringstream ss;
      ss << "readMeshFaceList: ";
      ss << "Can't find header: " << MESH_FACE_LIST_HEADER;
    }

    std::vector<Face> faces{};

    while (std::getline(is, line)) {
      std::stringstream ss;
      ss << line;

      std::string name;
      ss >> name;
      if (name != MESH_FACE_HEADER) {
        break;
      }

      Face f;
      ss >> f;

      faces.emplace_back(f);
    }

    return faces;
  }

  static auto saveInfo(std::ostream &ios, std::size_t num_vertex,
                       std::size_t num_faces) -> void {
    ios << (num_vertex) << "\n";
    ios << (num_faces) << "\n";
  }

  static auto saveElements(std::ostream &os,
                           const std::vector<Face> &faces) -> void {
    for (const auto &f : faces) {
      os << (f.v1()) << " " << (f.v2()) << " " << (f.v3()) << "\n";
    }
  }

  static auto saveVertices(std::ostream &os,
                           const std::vector<Vertex> &vertices) -> void {
    for (const auto &v : vertices) {
      os << v.x() << " " << v.y() << " " << v.z() << "\n";
    }
  }
};

}  // namespace ase_reader

#endif  // __ASE_READER_ASE_MESH_HPP__
