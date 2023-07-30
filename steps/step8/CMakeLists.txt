set(NX 31)
set(NY 31)
set(NT 100)
set(NU 0.05)
set(SIGMA 0.25)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step8_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step8_params.h
	@ONLY)


add_executable(step8 step8.cpp ${CMAKE_CURRENT_BINARY_DIR}/step8_params.h)
target_include_directories(step8 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step8 util)