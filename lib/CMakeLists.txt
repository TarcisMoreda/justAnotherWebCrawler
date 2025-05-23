cmake_minimum_required(VERSION 4.0.0)

project(jawc)

include(FetchContent)

FetchContent_Declare(
    lexbor
    GIT_REPOSITORY  "https://github.com/lexbor/lexbor.git"
    GIT_TAG         "v2.4.0"
    CMAKE_ARGS      -DBUILD_SHARED_LIBS=OFF
)
FetchContent_MakeAvailable(lexbor)

FetchContent_Declare(
    curl
    GIT_REPOSITORY  "https://github.com/curl/curl.git"
    GIT_TAG         "curl-8_13_0"
    CMAKE_ARGS      -DBUILD_SHARED_LIBS=OFF -DHTTP_ONLY=ON
)
FetchContent_MakeAvailable(curl)

add_library(${PROJECT_NAME} STATIC jawc.c include/jawc.h)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE lexbor libcurl)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)