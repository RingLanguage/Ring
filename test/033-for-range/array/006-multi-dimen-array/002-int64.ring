package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var int64[,] local_bool_array_0 = int64[,]{
		int64[]{1L, 2L, 3L},
		int64[]{4L, 5L, 6L},
		int64[]{7L, 8L, 9L},
		int64[]{10L, 11L, 12L},
	};

	var int index;
	var int64[] value;
	
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