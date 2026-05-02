qt_add_library(entrypoint STATIC
        entrypoint.h
)

target_include_directories(entrypoint INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})