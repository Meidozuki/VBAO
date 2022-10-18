
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt configure
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Qt/6.2.4/mingw_64/lib/cmake")

find_package(Qt6 REQUIRED
        COMPONENTS
        Core
        Gui
        Widgets
        )