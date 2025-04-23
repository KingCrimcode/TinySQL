cmake_minimum_required(VERSION 4.0)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} VER)

project(TinySQL VERSION ${VER})

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(TINYSQL_VERSION="${VER}")

add_compile_options(
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-unused-value
  -Wno-missing-field-initializers
  -Wno-narrowing
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
bison_target(Parser src/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/headers/parser.hpp)
flex_target(Lexer src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/headers/lexer.hpp)
add_flex_bison_dependency(Lexer Parser)

file(GLOB_RECURSE SRCFILES
  "src/*.cpp"
  "src/parser/*.cpp"
  "src/repl/*.cpp"
  "src/utils/*.cpp"
)

add_executable(
  tinysql
  ${SRCFILES}
  ${BISON_Parser_OUTPUTS}
  ${FLEX_Lexer_OUTPUTS}
)

target_include_directories(tinysql PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/parser
  ${CMAKE_SOURCE_DIR}/src/repl
  ${CMAKE_SOURCE_DIR}/src/utils
  ${CMAKE_CURRENT_BINARY_DIR}/headers
)
