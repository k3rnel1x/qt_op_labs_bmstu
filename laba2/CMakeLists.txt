cmake_minimum_required(VERSION 3.16)
project(laba2 VERSION 0.1 LANGUAGES CXX)

#set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_BUILD_TYPE "DEBUG")
set(ENABLE_PRICOLCHICKI 0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        ui/graph.h
        logic/appcontext.h
        logic/logic.cpp
        logic/logic.h
        logic/result.h
        logic/array.cpp
        logic/array.h
        ui/main.cpp
        ui/mainwindow.cpp
        ui/mainwindow.h
        ui/mainwindow.ui
        logic/entrypoint.h
        logic/entrypoint.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(laba2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(laba2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(laba2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(laba2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets stdc++ m)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.laba2)
endif()
set_target_properties(laba2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${maPROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS laba2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    target_compile_definitions(laba2 PRIVATE DEBUG)
endif()

if( NOT(CMAKE_BUILD_TYPE STREQUAL "RELEASE") AND ( ENABLE_PRICOLCHICKI ) )
    target_compile_definitions(laba2 PRIVATE PRICOLCHICKI)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(laba2)
endif()
