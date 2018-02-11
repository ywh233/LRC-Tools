cmake_minimum_required(VERSION 3.10.0)
project(LRCTools VERSION 1.0.0)

add_compile_options(-std=c++14)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_subdirectory(deps)
add_subdirectory(liblrc)
add_subdirectory(cli)
add_subdirectory(test)
