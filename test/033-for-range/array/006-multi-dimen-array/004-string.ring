package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var string[,] local_bool_array_0 = string[,]{
		string[]{"a", "b", "c"},
		string[]{"d", "e", "f"},
		string[]{"g", "h", "i"},
		string[]{"j", "k", "l"},
		string[]{"m", "n", "o"},
		string[]{"p", "q", "r"},
		string[]{"s", "t", "u"},
		string[]{"v", "w", "x"},
		string[]{"y", "z"},
	};

	var int index;
	var string[] value;
	
	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		fmt::println(value);
	}

	fmt::println_string("\ntest case for --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		fmt::println(local_bool_array_0[index]);
	}

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0;
	for _, value = range local_bool_array_0 {
		index++;
	}

	fmt::println_string("\ntest case range-index --------------------");
	debug::assert(index == len(local_bool_array_0));
}