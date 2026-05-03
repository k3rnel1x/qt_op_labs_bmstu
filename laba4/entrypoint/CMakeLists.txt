qt_add_library(entrypoint STATIC
        entrypoint.cpp
)
target_link_libraries(entrypoint PRIVATE businesslogic)
target_include_directories(entrypoint INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})