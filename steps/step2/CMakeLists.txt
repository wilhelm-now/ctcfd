set(NX 41)
set(NT 20)
set(DT 0.025)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step2_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step2_params.h
	@ONLY)
add_executable(step2 step2.cpp ${CMAKE_CURRENT_BINARY_DIR}/step2_params.h)
target_include_directories(step2 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step2 util)