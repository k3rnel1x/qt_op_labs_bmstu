qt_add_library(businesslogic STATIC
        businesslogic.cpp
)

target_link_libraries(businesslogic PRIVATE entrypoint Qt6::Core Qt6::Widgets)
target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})