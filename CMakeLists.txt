cmake_minimum_required(VERSION 3.26) #probably works on older, I just haven't tried.

project(ctcfd)

set(CMAKE_CXX_STANDARD 98) 	# todo: better than this.


add_library(util INTERFACE)
target_include_directories(util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/util)

add_subdirectory(application)

