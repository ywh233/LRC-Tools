# lrc_metadata
file(GLOB SRCS_LRC_METADATA
    "lrc_metadata.cc")
add_executable(lrc-metadata ${SRCS_LRC_METADATA})
add_dependencies(lrc-metadata
    cxxopts
    lrc)
target_link_libraries(lrc-metadata lrc)

# lrc_info
file(GLOB SRCS_LRC_LOOKUP
    "lrc_lookup.cc")
add_executable(lrc-lookup ${SRCS_LRC_LOOKUP})
add_dependencies(lrc-lookup
    cxxopts
    lrc)
target_link_libraries(lrc-lookup lrc)
