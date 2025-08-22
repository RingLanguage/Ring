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
	complex_math();
}


fn complex_math() {
	fmt::printf("\ncomplex_math-------\n");

	global_int_value = 1+ @int(2L*2L);
	debug::assert(global_int_value == 5);
	fmt::printf("global_int_value = {}\n", global_int_value);

	global_double_value = @double(10)/2.1;
	debug::assert(global_double_value == 4.761904761904762);
	fmt::printf("global_double_value = {}\n", global_double_value);

}


