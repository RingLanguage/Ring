package main

import {
	debug;
	fmt;
}

global {

}

// TE: 对于 int 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var int value;

	fmt::println_string("test case for-range --------------------");
	for index, value = range int[]{0, 1, 2, 
		3, 4, 5, 
		6, 7, 8, 
		9, 10, 11, 
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29} {
		fmt::println_int(value);
	}
	debug::assert(index == 29L);

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range int[]{0, 1, 2, 
		3, 4, 5, 
		6, 7, 8, 
		9, 10, 11, 
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29} {
		index++;
	}
	debug::assert(index == 30L);
}