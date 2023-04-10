cmake_minimum_required(VERSION 3.16)
project(VBAO_py LANGUAGES CXX)

#include pybind11 package
option(BUILD_PYTHON_LIB "whether to build with pybind11" TRUE)

include_directories(${CMAKE_SOURCE_DIR}/extern/pybind11/include)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

IF(BUILD_PYTHON_LIB)
    pybind11_add_module(VBAO_py
            VBAO/VBaoBase.hpp VBAO/command.hpp VBAO/listener.hpp
            VBAO/model.hpp VBAO/viewmodel.hpp build_python.cpp)
    target_compile_definitions(VBAO_py PRIVATE BUILD_PYBIND=$<BOOL:${BUILD_PYBIND}>)
    set_target_properties(VBAO_py PROPERTIES OUTPUT_NAME vbao)

ENDIF()