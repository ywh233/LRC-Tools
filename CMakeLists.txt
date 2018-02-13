cmake_minimum_required(VERSION 3.2.0)
project(LRCTools VERSION 1.0.0)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-missing-braces")

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_subdirectory(third_party)
add_subdirectory(liblrc)
add_subdirectory(cli)
add_subdirectory(test)
