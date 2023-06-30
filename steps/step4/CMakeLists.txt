set(LX 6.283185307179586) # 2PI long, need domain length in cpp and py, therefore define here
set(NX 101)
set(NT 99)
set(NU 0.07)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step4_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step4_params.h
	@ONLY)

add_executable(step4 step4.cpp ${CMAKE_CURRENT_BINARY_DIR}/step4_params.h)
target_include_directories(step4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step4 util)

macro(make_step4_header outname timesteps)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${outname}
        COMMAND python step4_analytical.py -o ${CMAKE_CURRENT_BINARY_DIR}/${outname} --NX ${NX} --LX ${LX} --NU ${NU} --NT ${timesteps}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_sources(step4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${outname})
endmacro()
# create initial conditions header
make_step4_header(step4_initial.h 0)
make_step4_header(step4_final.h ${NT})
