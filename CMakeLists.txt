cmake_minimum_required(VERSION 3.26) #probably works on older, I just haven't tried.

project(ctcfd)

set(CMAKE_CXX_STANDARD 98) 	# todo: better than this.


add_library(util INTERFACE)
target_include_directories(util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/util)
if (MSVC)
	target_compile_options(util INTERFACE /Zc:enumTypes) # make sure number type doesn't roll over after INT_MAX
endif()

add_subdirectory(steps)

if (PROJECT_IS_TOP_LEVEL)
	enable_testing()
	add_subdirectory(util/tests)
endif()
