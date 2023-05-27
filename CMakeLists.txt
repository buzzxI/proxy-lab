cmake_minimum_required(VERSION 3.10)

project(proxy)

add_subdirectory(sbuf)
add_subdirectory(map)

add_executable(proxy proxy.c)

target_link_libraries(proxy libsbuf libmap csapp)
