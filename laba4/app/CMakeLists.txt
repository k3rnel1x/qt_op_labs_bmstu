qt_add_executable(laba4
    mainwindow.cpp
    mainwindow.h
    surfacedrawer.h
    surfacedrawer.cpp
    logger.hpp
    appcontext.h
    main.cpp
)

qt_add_ui(laba4 SOURCES mainwindow.ui)

target_link_libraries(laba4 PRIVATE businesslogic entrypoint Qt6::Widgets stdc++ Qt6::Core)
target_include_directories(laba4 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(laba4 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)