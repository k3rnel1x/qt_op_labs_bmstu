cmake_minimum_required(VERSION 3.16)

project(laba4 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
        Widgets
        DataVisualization
        3DCore
        3DRender
        3DInput
        3DLogic
        3DExtras
)


qt_standard_project_setup()

add_subdirectory(entrypoint)
add_subdirectory(businesslogic)

qt_add_executable(laba4
        app/main.cpp
        gui/mainwindow.cpp
        gui/mainwindow.h
        gui/mainwindow.ui
        app/appcontext.h
        gui/SurfaceDrawer.cpp
        gui/SurfaceDrawer.h
)

target_link_libraries(laba4 PRIVATE
        entrypoint
        businesslogic

        Qt6::Widgets
        Qt6::DataVisualization
        stdc++
        Qt6::3DCore
        Qt6::3DRender
        Qt6::3DLogic
        Qt6::3DExtras
)