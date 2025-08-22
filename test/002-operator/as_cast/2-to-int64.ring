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
	test_int_2_int64();
}


// 测试数值边界情况
fn test_int_2_int64() {
	fmt::printf("\ntest_int_2_int64-------\n");

	global_int_value = 1;
	global_int64_value = @int64(global_int_value);
	debug::assert(global_int64_value == 1L);
	fmt::printf("int64(max int) = {}\n", global_int64_value);

	global_int_value = 12345678;
	global_int64_value = @int64(global_int_value);
	debug::assert(global_int64_value == 12345678L);
	fmt::printf("int64(max int) = {}\n", global_int64_value);

	global_int_value = -12345678;  // 32位int最大值
	global_int64_value = @int64(global_int_value);
	debug::assert(global_int64_value == -12345678L);
	fmt::printf("int64(max int) = {}\n", global_int64_value);

	// 最大整数转换
	global_int_value = 2147483647;  // 32位int最大值
	global_int64_value = @int64(global_int_value);
	debug::assert(global_int64_value == 2147483647L);
	fmt::printf("int64(max int) = {}\n", global_int64_value);

	// 最小负数转换
	global_int_value = -2147483648;
	global_int64_value = @int64(global_int_value);
	debug::assert(global_int64_value == -2147483648L);
	fmt::printf("int64(min int) = {}\n", global_int64_value);
}

