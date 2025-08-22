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
	test_numeric_limits_from_int64();
}

// 测试数值边界情况
fn test_numeric_limits_from_int64() {
	fmt::printf("\ntest_numeric_limits_from_int64-------\n");

	global_int64_value = 0L;
	global_double_value = @double(global_int64_value);
	debug::assert(global_double_value == 0.0);
	fmt::printf("double(0L) = {}\n", global_double_value);

	global_int64_value = 9007199254740991L;
	global_double_value = @double(global_int64_value);
	debug::assert(global_double_value == 9007199254740991.0);
	fmt::printf("double(9007199254740991L) = {}\n", global_double_value);

	global_int64_value = 9007199254740992L;
	global_double_value = @double(global_int64_value);
	debug::assert(global_double_value == 9007199254740992.0);
	fmt::printf("double(9007199254740992L) = {}\n", global_double_value);

	// 大整数转double可能丢失精度
	global_int64_value = 9007199254740993L;  // 2^53 + 1
	global_double_value = @double(global_int64_value);
	debug::assert(global_double_value != 9007199254740991.0);
	debug::assert(global_double_value == 9007199254740992.0);
	debug::assert(global_double_value == 9007199254740993.0);
	debug::assert(global_double_value != 9007199254740994.0);
	fmt::printf("double(9007199254740993L) = {}\n", global_double_value);


	// int64 max
	global_double_value = @double(9223372036854775807L);
	debug::assert(global_double_value == 9223372036854775806.0);
	debug::assert(global_double_value == 9223372036854775807.0);
	debug::assert(global_double_value == 9223372036854775808.0);

	// int64 min
	global_double_value = @double(-9223372036854775808L);
	debug::assert(global_double_value == -9223372036854775807.0);
	debug::assert(global_double_value == -9223372036854775808.0);
	debug::assert(global_double_value == -9223372036854775809.0);

}