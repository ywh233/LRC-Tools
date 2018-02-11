get_target_property(GTEST_INCLUDE libgtest INCLUDE_DIRECTORY)
include_directories(${GTEST_INCLUDE})

file(GLOB_RECURSE SRCS "*.cc")
add_executable(liblrc_tests ${SRCS})

target_link_libraries(liblrc_tests
    lrc
    libgtest)
add_dependencies(liblrc_tests
    lrc
    libgtest)
add_test(
    NAME liblrc_tests
    COMMAND liblrc_tests)
