package main

import {
	debug;
	fmt;
}

global {

}

typedef FuncType = fn();

// TE: 对于 匿名函数 数组的 for-range 测试，range的对象是个变量
fn main() {
	var FuncType[] local_bool_array_0 = FuncType[]{
		fn() { fmt::println_string("Function 0 executed"); },
		fn() { fmt::println_string("Function 1 executed"); },
		fn() { fmt::println_string("Function 2 executed"); },
		fn() { fmt::println_string("Function 3 executed"); },
		fn() { fmt::println_string("Function 4 executed"); },
		fn() { fmt::println_string("Function 5 executed"); },
	};

	var int index;
	var FuncType value;
	
	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		value();
	}

	fmt::println_string("\ntest case for --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		local_bool_array_0[index]();
	}

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0;
	for _, value = range local_bool_array_0 {
		index++;
	}

	fmt::println_string("\ntest case range-index --------------------");
	debug::assert(index == len(local_bool_array_0));
}