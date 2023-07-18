macro(make_util_test component)
	add_executable(test-${component} ${component}.test.cpp)
	target_link_libraries(test-${component} util)
	add_test(
		NAME test-${component}
		WORKING_DIRECTORY $<TARGET_FILE_DIR:test-${component}>
		COMMAND $<TARGET_FILE_NAME:test-${component}>
	)
endmacro()

make_util_test(for_i)
make_util_test(is_same)
make_util_test(ones)
make_util_test(range)
make_util_test(diff)