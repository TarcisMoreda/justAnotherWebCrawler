cmake_minimum_required(VERSION 4.0.0)

project(jawc)
add_executable(${PROJECT_NAME} src/main.c)

add_subdirectory(submodules/curl)

include(ExternalProject)
ExternalProject_Add(
  tidy-html5-external
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules/tidy-html5"  # Path to submodule
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/tidy-install  # Install locally
    -DBUILD_SHARED_LIB=OFF  # Optional: Build static lib
  INSTALL_COMMAND ""  # Skip install (we'll manually reference outputs)
  BUILD_BYPRODUCTS 
    ${CMAKE_BINARY_DIR}/tidy-install/lib/libtidy.a  # Static lib path
)
add_library(tidy-html5 STATIC IMPORTED)
set_target_properties(tidy-html5 PROPERTIES
  IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/tidy-install/lib/libtidy.a
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/tidy-install/include
)

add_dependencies(jawc tidy-html5-external)

target_link_libraries(jawc PRIVATE tidy-html5)