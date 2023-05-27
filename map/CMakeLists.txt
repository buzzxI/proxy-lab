add_library(libmap STATIC map.c)

target_include_directories(libmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(libmap PRIVATE csapp)