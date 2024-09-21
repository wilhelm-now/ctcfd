# Helper for creating successive-over-relaxation header
find_package(Python)

cmake_path(SET CTCFD_SCRIPT_DIR NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}/../scripts")

function(MAKE_SOR_HEADER)
	if (NOT Python_FOUND)
		message(FATAL_ERROR "Can't create successive-over-relaxation header without python")
	endif()

	set(options)
	set(one_value_args OUTPUT NX NY LX LY PARAMNAME)
	set(multi_value_args)
	cmake_parse_arguments(SOR_ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

	add_custom_command(
		OUTPUT ${SOR_ARGS_OUTPUT}
		COMMAND ${Python_EXECUTABLE} sor_omega.py -o ${SOR_ARGS_OUTPUT} --NX ${SOR_ARGS_NX} --NY ${SOR_ARGS_NY} --LX ${SOR_ARGS_LX} --LY ${SOR_ARGS_LY} --paramname ${SOR_ARGS_PARAMNAME}
		WORKING_DIRECTORY ${CTCFD_SCRIPT_DIR}
	)
endfunction()

function(ADD_PLOT_FLOW_TARGET)
	set(options)
	set(one_value_args SOURCE_TARGET)
	set(multi_value_args PLOT_ARGS)
	cmake_parse_arguments(TARGET_ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

	add_custom_target(plot-${TARGET_ARGS_SOURCE_TARGET}
		DEPENDS ${TARGET_ARGS_SOURCE_TARGET}
		COMMAND $<TARGET_FILE_NAME:${TARGET_ARGS_SOURCE_TARGET}> | $<SHELL_PATH:${Python_EXECUTABLE}> $<SHELL_PATH:${CTCFD_SCRIPT_DIR}/plot_flow.py> ${TARGET_ARGS_PLOT_ARGS}
		WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET_ARGS_SOURCE_TARGET}>
	)	
endfunction()

# add all steps
foreach(i RANGE 1 11)
	add_subdirectory(step${i})
endforeach()
