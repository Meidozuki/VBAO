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

set(SRC_LIST
        example/model/model.cpp
        example/viewmodel/viewmodel.cpp example/viewmodel/viewmodel_listener.cpp example/viewmodel/viewmodel_command.cpp
        example/view/displayer.cpp
        example/window/window.cpp example/window/window_listener.cpp
        example/app/main.cpp example/app/app.cpp)

#Unicode
if(WIN32)
    set(STRING_CXX_FLAGS "-D_UNICODE")
    foreach(src ${SRC_CPP})
        set_source_files_properties(${src} PROPERTIES
                COMPILE_FLAGS "${STRING_CXX_FLAGS}"
                )
    endforeach()
endif(WIN32)

add_executable(VBAOTest
        ${SRC_LIST})

target_link_libraries(VBAOTest PUBLIC
        Qt::Gui
        Qt::Widgets
        )

set_target_properties(VBAOTest PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS VBAOTest
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VBAOTest)
endif()

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