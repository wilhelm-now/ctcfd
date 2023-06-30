set(LX 6.283185307179586) # 2PI long, need domain length in cpp and py, therefore define here
set(NX 101)
set(NT 100)
set(NU 0.07)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step4_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step4_params.h
	@ONLY)

# COMMAND python step_inital.py -o ${CMAKE_CURRENT_BINARY_DIR}/step4_initial.h --NX ${NX} --NU ${NU}
# create initial conditions header
add_custom_target(step4_initial_conditions ALL
    COMMAND python step4_analytical.py -o ${CMAKE_CURRENT_BINARY_DIR}/step4_initial.h --NX ${NX} --LX ${LX} --NU ${NU}
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/step4_initial.h 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

add_executable(step4 step4.cpp ${CMAKE_CURRENT_BINARY_DIR}/step4_params.h ${CMAKE_CURRENT_BINARY_DIR}/step4_initial.h)
target_include_directories(step4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step4 util)