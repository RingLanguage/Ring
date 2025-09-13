package main

import {
	debug;
	fmt;
}

global {

}

// TE: 对于 bool 数组的 for-range 测试，range的对象是个变量
fn main() {
	var bool[] local_bool_array_0 = bool[]{true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false};

	var int index;
	var bool value;
	
	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		fmt::println_bool(value);
	}

	fmt::println_string("\ntest case for --------------------");
	for index = 0; index<30; index++ {
		fmt::println_bool(local_bool_array_0[index]);
	}

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0;
	for _, value = range local_bool_array_0 {
		debug::assert(value == local_bool_array_0[index]);
		index++;
	}

	fmt::println_string("\ntest case range-index --------------------");
	debug::assert(index == 30);
}