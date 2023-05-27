add_library(libsbuf STATIC sbuf.c)

target_include_directories(libsbuf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(libsbuf PRIVATE csapp)