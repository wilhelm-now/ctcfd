set(C 1)
set(NX 41)
set(DT 0.025)
set(NT 25)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step1_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step1_params.h
	@ONLY)
add_executable(step1 step1.cpp ${CMAKE_CURRENT_BINARY_DIR}/step1_params.h)
target_include_directories(step1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step1 util)
