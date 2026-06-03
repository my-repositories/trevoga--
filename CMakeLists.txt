cmake_minimum_required(VERSION 3.15)

project(TrevogaApp CXX)

set(CMAKE_CXX_STANDARD 11)


add_compile_definitions(_AMD64_)
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/utf-8>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(include)

add_subdirectory(src)

list(TRANSFORM PROJECT_SOURCES PREPEND "src/")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)