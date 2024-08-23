add_executable(read_ase_file main.cpp)

# flag for wall and wextra and pedantic
target_compile_options(read_ase_file PRIVATE -Wall -Wextra -pedantic -Wall -Wextra -Werror)
target_include_directories(read_ase_file PRIVATE ${PROJECT_SOURCE_DIR}/include)
