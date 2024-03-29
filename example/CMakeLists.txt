cmake_minimum_required(VERSION 3.16)
project(VBAOTest)
set(CMAKE_CXX_STANDARD 17)

set(SRC_LIST
        model/model.cpp
        viewmodel/viewmodel.cpp viewmodel/viewmodel_listener.cpp viewmodel/viewmodel_command.cpp
        view/displayer.cpp
        window/window.cpp window/window_listener.cpp
        app/main.cpp app/app.cpp)

#Unicode
if(WIN32)
    set(STRING_CXX_FLAGS "-D_UNICODE")
    foreach(src ${SRC_CPP})
        set_source_files_properties(${src} PROPERTIES
                COMPILE_FLAGS "${STRING_CXX_FLAGS}"
                )
    endforeach()
endif(WIN32)

find_package(Qt MODULE)

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