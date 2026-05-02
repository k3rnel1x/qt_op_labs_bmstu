qt_add_library(businesslogic STATIC
        businesslogic.h
)

target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})