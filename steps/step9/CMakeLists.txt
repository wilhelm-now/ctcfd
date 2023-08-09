set(NX 31)
set(NY 31)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step9_params.h.in
	${CMAKE_CURRENT_BINARY_DIR}/step9_params.h
	@ONLY)

add_executable(step9 step9.cpp ${CMAKE_CURRENT_BINARY_DIR}/step9_params.h)
target_include_directories(step9 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step9 util)