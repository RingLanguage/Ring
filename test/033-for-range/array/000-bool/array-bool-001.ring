package main

import {
	debug;
	fmt;
}

global {

}

// TE: 对于 bool 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var bool value;
	
	fmt::println_string("test case for-range --------------------");
	for index, value = range bool[]{true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false} {
		fmt::println_bool(value);
	}
	debug::assert(index == 29L);

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range bool[]{true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false, 
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false,
		true, false, false} {
		index++;
	}
	debug::assert(index == 30L);
}