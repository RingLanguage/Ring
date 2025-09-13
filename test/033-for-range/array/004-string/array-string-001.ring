package main

import {
	debug;
	fmt;
}

global {

}

// TE: 对于 string 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var string value;
	
	fmt::println_string("test case for-range --------------------");
	for index, value = range string[]{"0.0", "1.0", "2.0", 
		"3.0", "4.0", "5.0", 
		"6.0", "7.0", "8.0", 
		"9.0", "10.0", "11.0", 
		"12.0", "13.0", "14.0",
		"15.0", "16.0", "17.0",
		"18.0", "19.0", "20.0",
		"21.0", "22.0", "23.0",
		"24.0", "25.0", "26.0",
		"27.0", "28.0", "29.0"} {
		fmt::println_string(value);
	}
	debug::assert(index == 29L);

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range string[]{"0.0", "1.0", "2.0", 
		"3.0", "4.0", "5.0", 
		"6.0", "7.0", "8.0", 
		"9.0", "10.0", "11.0", 
		"12.0", "13.0", "14.0",
		"15.0", "16.0", "17.0",
		"18.0", "19.0", "20.0",
		"21.0", "22.0", "23.0",
		"24.0", "25.0", "26.0",
		"27.0", "28.0", "29.0"} {
		index++;
	}
	debug::assert(index == 30L);
}