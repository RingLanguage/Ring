package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var int[,] local_bool_array_0 = int[,]{
		int[]{1, 2, 3, 4, 5},
		int[]{6, 7, 8, 9, 10},
		int[]{11, 12, 13, 14, 15},
		int[]{16, 17, 18, 19, 20},
	};

	var int index;
	var int[] value;
	
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