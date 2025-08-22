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


// TE: 测试类型别名转换
fn main() {
	test_cast_type_alias_int();
}


typedef IntAlias = int;
fn test_cast_type_alias_int() {
	fmt::printf("\ntest_cast_type_alias-------\n");

	var int          int_value 		 = 1;
	var IntAlias     int_alias_value = 0;

	fmt::printf("int_value = {}\n", int_value);
	fmt::printf("int_alias_value = {}\n", int_alias_value);
	debug::assert(int_value != int_alias_value);


	int_alias_value = @IntAlias(101);
	debug::assert(int_alias_value == 101);

	int_value = @int(int_alias_value);
	debug::assert(int_value == 101);

	fmt::printf("int_value = {}\n", int_value);
	fmt::printf("int_alias_value = {}\n", int_alias_value);
	debug::assert(int_value == int_alias_value);
}