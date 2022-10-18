cmake_minimum_required(VERSION 3.16)
project(libtest)
set(CMAKE_CXX_STANDARD 17)

# include lib
include_directories("D:/coding/mvvm/library")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "D:/coding/qt/qt_template")

find_package(Qt MODULE)

qt_add_executable(libtest
        MANUAL_FINALIZATION
        main.cpp view.cpp view.h view.ui)

target_link_libraries(libtest PUBLIC
        Qt::Gui
        Qt::Widgets
        )

set_target_properties(libtest PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS libtest
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(libtest)
endif()

