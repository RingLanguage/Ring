package main

import {
	debug;
	fmt;
}

global {

}

// TE: 对于 int64 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var int64 value;

	fmt::println_string("test case for-range --------------------");
	for index, value = range int64[]{0L, 1L, 2L, 
		3L,  4L,  5L, 
		6L,  7L,  8L, 
		9L,  10L, 11L, 
		12L, 13L, 14L,
		15L, 16L, 17L,
		18L, 19L, 20L,
		21L, 22L, 23L,
		24L, 25L, 26L,
		27L, 28L, 29L} {
		fmt::println_int64(value);
	}
	debug::assert(index == 29L);


	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range int64[]{0L, 1L, 2L, 
		3L,  4L,  5L, 
		6L,  7L,  8L, 
		9L,  10L, 11L, 
		12L, 13L, 14L,
		15L, 16L, 17L,
		18L, 19L, 20L,
		21L, 22L, 23L,
		24L, 25L, 26L,
		27L, 28L, 29L} {
		index++;
	}
	debug::assert(index == 30L);
}