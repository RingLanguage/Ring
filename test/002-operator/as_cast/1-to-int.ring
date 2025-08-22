package main

import {
	fmt;
	debug;
}

global {
	var bool 	global_bool_value;
	var int 	global_int_value;
	var int64 	global_int64_value;
	var double 	global_double_value;
	var string 	global_string_value;
}


fn main() {
	test_from_bool_to_int();
}

fn test_from_bool_to_int() {
	fmt::printf("\ntest_from_bool_to_int-------\n");

	global_int_value = @int(true);
	debug::assert(global_int_value == 1);

	global_int_value = @int(false);
	debug::assert(global_int_value == 0);



	global_int_value = @int(not true);
	debug::assert(global_int_value == 0);

	global_int_value = @int(not false);
	debug::assert(global_int_value == 1);



	global_int_value = @int(not not true);
	debug::assert(global_int_value == 1);

	global_int_value = @int(not not false);
	debug::assert(global_int_value == 0);


	global_int_value = @int(global_bool_value);
	debug::assert(global_int_value == 0);

	global_int_value = @int(not global_bool_value);
	debug::assert(global_int_value == 1);
}


fn test_from_complex_expression_to_bool() {
	fmt::printf("\ntest_from_complex_expression_to_bool-------\n");

}