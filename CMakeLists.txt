cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -pedantic ${CMAKE_CXX_FLAGS}")

add_subdirectory(pe)
add_subdirectory(unittest)
add_subdirectory(third/jsoncpp)
add_subdirectory(policymgr)
add_subdirectory(tool)
