cmake_minimum_required(VERSION 3.16)
project(libtest)
set(CMAKE_CXX_STANDARD 17)

# include lib
include_directories("./library")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "D:/coding/qt/qt_template")
find_package(Qt MODULE)

set(SRC_LIST
        main.cpp displayer.cpp displayer.h displayer.ui
        model/model.cpp
        viewmodel/viewmodel.cpp viewmodel/viewmodel_sink.cpp viewmodel/viewmodel_command.cpp
        window/window.cpp window/window_listener.cpp view/view.cpp)

add_executable(libtest
        ${SRC_LIST})

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

