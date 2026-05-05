qt_add_library(businesslogic STATIC
        businesslogic.cpp
        pointsarr/pointsarr.h
        pointsarr/pointsarr.cpp
        pointsarr/point.h
)

target_link_libraries(businesslogic PRIVATE entrypoint Qt6::Core Qt6::Widgets)
target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})