cmake_minimum_required(VERSION 3.16)

project(laba4 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets COMPONENTS Core)

qt_standard_project_setup()

add_compile_definitions(DIRECTORY="/home/k3rnel1x/Programing/qt_op_labs_bmstu/laba4")

add_subdirectory(app)
add_subdirectory(entrypoint)
add_subdirectory(businesslogic)