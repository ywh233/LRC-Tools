include(ExternalProject)

# Download & Build
ExternalProject_Add(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v2.0.0
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/cxxopts
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(cxxopts source_dir)

set_target_properties(cxxopts PROPERTIES
    "INCLUDE_DIRECTORY" "${source_dir}/include")
