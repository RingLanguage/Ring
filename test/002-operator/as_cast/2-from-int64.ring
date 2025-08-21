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
	test_int64_2_bool();
	test_int64_2_int();
	test_int64_2_int64();
	test_int64_2_double();
	test_int64_2_string();
}

fn test_int64_2_bool() {
	fmt::printf("\ntest_int64_2_bool-------\n");

	global_bool_value = @bool(123L);
	fmt::printf("global_bool_value = {}\n", global_bool_value);

	global_bool_value = @bool(0L);
	fmt::printf("global_bool_value = {}\n", global_bool_value);
}
fn test_int64_2_int() {
	fmt::printf("\ntest_int64_2_int-------\n");

	global_int_value = @int(123L);
	fmt::printf("global_int_value = {}\n", global_int_value);

	global_int_value = @int(0L);
	fmt::printf("global_int_value = {}\n", global_int_value);
}
fn test_int64_2_int64() {
	fmt::printf("\ntest_int64_2_int64-------\n");

	global_int64_value = @int64(123L);
	fmt::printf("global_int64_value = {}\n", global_int64_value);

	global_int64_value = @int64(0L);
	fmt::printf("global_int64_value = {}\n", global_int64_value);
}
fn test_int64_2_double() {
	fmt::printf("\ntest_int64_2_double-------\n");

	global_double_value = @double(123L);
	fmt::printf("global_double_value = {}\n", global_double_value);

	global_double_value = @double(0L);
	fmt::printf("global_double_value = {}\n", global_double_value);
}
fn test_int64_2_string() {
	fmt::printf("\ntest_int64_2_string-------\n");

	global_string_value = @string(1L);
	fmt::printf("global_string_value = {}\n", global_string_value);

	global_string_value = @string(9223372036854775807L);
	fmt::printf("global_string_value = {}\n", global_string_value);

	global_string_value = @string(-9223372036854775808L);
	fmt::printf("global_string_value = {}\n", global_string_value);
}

