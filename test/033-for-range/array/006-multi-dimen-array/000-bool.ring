package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var bool[,] local_bool_array_0 = bool[,]{
		bool[]{true, false, false, true, false},
		bool[]{false, true, false, true, false},
		bool[]{true, false, true, false, true},
		bool[]{false, true, false, true, false},
	};

	var int index;
	var bool[] value;
	
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