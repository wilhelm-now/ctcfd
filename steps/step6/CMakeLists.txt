set(NX 81)
set(NY 81)
set(NT 100)
set(SIGMA 0.2)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step6_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step6_params.h
	@ONLY)


add_executable(step6 step6.cpp ${CMAKE_CURRENT_BINARY_DIR}/step6_params.h)
target_include_directories(step6 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step6 util)


add_executable(step6-tracked step6.cpp ${CMAKE_CURRENT_BINARY_DIR}/step6_params.h)
target_include_directories(step6-tracked PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step6-tracked util)
target_compile_definitions(step6-tracked PUBLIC CTCFD_FASTTRACKED)