#include <ase_reader/ase_reader.hpp>
#include <cstdlib>

auto main(int argc, char** argv) -> int {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>"
              << "\n";
    return 1;
  }

  ase_reader::ASEReader ase;
  ase.read(argv[1]);

  std::filesystem::path save_dir{"./tmp"};
  save_dir /= std::filesystem::path{argv[1]}.stem();

  ase.setPrecision(6);

  // auto info_os = std::fstream{save_dir / "info.txt",std::ios::in |
  // std::ios::out | std::ios::trunc}; auto vertices_os = std::ofstream{save_dir
  // / "vertices.txt", std::ios::out}; auto elements_os = std::ofstream{save_dir
  // / "elements.txt", std::ios::out};

  // ase.save(info_os, vertices_os, elements_os);

  // ase.save(save_dir.string());
  ase.setPrecision(6);

  for (const auto& object : ase.objects()) {
    if (!std::filesystem::exists(save_dir / object.name())) {
      std::filesystem::create_directories(save_dir / object.name());
    }

    auto info_os = std::fstream{save_dir / object.name() / "info.txt",
                                std::ios::in | std::ios::out | std::ios::trunc};
    auto vertices_os =
        std::ofstream{save_dir / object.name() / "nodes.txt", std::ios::out};
    auto elements_os =
        std::ofstream{save_dir / object.name() / "elements.txt", std::ios::out};

    object.save(info_os, vertices_os, elements_os);
  }

  return 0;
}
