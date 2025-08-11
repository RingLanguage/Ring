package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var double[,] local_bool_array_0 = double[,]{
		double[]{1.1, 2.2, 3.3},
		double[]{4.4, 5.5, 6.6},
		double[]{7.7, 8.8, 9.9},
		double[]{10.10, 11.11, 12.12},
		double[]{13.13, 14.14, 15.15},
	};

	var int index;
	var double[] value;
	
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