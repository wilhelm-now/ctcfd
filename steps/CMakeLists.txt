# Helper for creating successive-over-relaxation header
find_package(Python)

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
		WORKING_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../scripts
	)
endfunction()

# add all steps
foreach(i RANGE 1 11)
	add_subdirectory(step${i})
endforeach()
