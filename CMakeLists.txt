cmake_minimum_required(VERSION 3.21)

project(hello-llvm
    VERSION 0.0.1
    DESCRIPTION "Hello World with LLVM"
    LANGUAGES C CXX
)

###############
# Conan Setup #
###############

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

###############
# Main Target #
###############

add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/main.c"
)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:DEBUG>:-fno-omit-frame-pointer -fsanitize=address>
)
target_link_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:DEBUG>:-fno-omit-frame-pointer -fsanitize=address>
)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} stdc++ CONAN_PKG::llvm-core)
