package main

import {
	debug;
	fmt;
}

global {

}

typedef FuncType = fn();

// TE: 对于 匿名函数 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var FuncType value;
	
	fmt::println_string("test case for-range --------------------");
	for index, value = range FuncType[]{
		fn() { fmt::println_string("Function 0 executed"); },
		fn() { fmt::println_string("Function 1 executed"); },
		fn() { fmt::println_string("Function 2 executed"); },
		fn() { fmt::println_string("Function 3 executed"); },
		fn() { fmt::println_string("Function 4 executed"); },
		fn() { fmt::println_string("Function 5 executed"); },
	} {
		value();
	}
	debug::assert(index == 5L);

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range FuncType[]{
		fn() { fmt::println_string("Function 0 executed"); },
		fn() { fmt::println_string("Function 1 executed"); },
		fn() { fmt::println_string("Function 2 executed"); },
		fn() { fmt::println_string("Function 3 executed"); },
		fn() { fmt::println_string("Function 4 executed"); },
		fn() { fmt::println_string("Function 5 executed"); },
	} {
		index++;
	}
	debug::assert(index == 6L);
}