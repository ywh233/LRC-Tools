file(GLOB_RECURSE SRCS "*.cc")
add_executable(liblrc_tests ${SRCS})

target_link_libraries(liblrc_tests
    lrc
    gtest)
add_test(
    NAME liblrc_tests
    COMMAND liblrc_tests)
