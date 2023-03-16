cmake_minimum_required(VERSION 3.16)
project(qtHello)
set(CMAKE_CXX_STANDARD 17)

# include lib
include_directories("../Lib_VBao")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(Qt MODULE)

qt_add_executable(qtHelloTest
        MANUAL_FINALIZATION
        main.cpp view.cpp view.h view.ui)

target_link_libraries(qtHelloTest PUBLIC
        Qt::Gui
        Qt::Widgets
        )

set_target_properties(qtHelloTest PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS qtHelloTest
        BUNDLE DESTINATION .)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qtHelloTest)
endif()

