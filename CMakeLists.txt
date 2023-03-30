cmake_minimum_required(VERSION 3.16)
project(VBAOTest)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()

# include the VBAO library
include_directories("Lib_VBao")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/qt-test)
find_package(Qt MODULE)

# test if qt is available
add_subdirectory(qt-test)

# load example
add_subdirectory(example)

# process VBAO library
file(GLOB Lib_files Lib_VBao/VBAO/*)
option(VBAO_BUILD_DYNAMIC "whether to build dynamic or static lib" FALSE)

if (VBAO_BUILD_DYNAMIC)
    add_library(VBAO SHARED ${Lib_files})
    install(TARGETS VBAO)
else()
    add_library(VBAO STATIC ${Lib_files})
    install(TARGETS VBAO)
endif()

install(FILES ${Lib_files} DESTINATION include)

include(CPack)

# add pybind
add_subdirectory(extern/pybind11)
add_subdirectory(Lib_VBao)