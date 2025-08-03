package main

import {
	debug;
	fmt;
}

global {

}

fn main() {
	var bool[] local_bool_array_0 = bool[]{true, false, false, 
		true, false, false};

	var int64 index;
	var bool value;


	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		fmt::printf("index={}, value={}\n", index, value);
	}

	fmt::println_string("test case for-range --------------------");
	value = false;
	for index, _ = range local_bool_array_0 {
		fmt::printf("index={}, value={}\n", index, value);
	}
	
	fmt::println_string("test case for-range --------------------");
	for index, value = range local_bool_array_0 {
		fmt::printf("index={}, value={}\n", index, value);
	}



	index = 100L;
	value = true;
	index, value = 123123L, true;
	fmt::printf("index={}, value={}\n", index, value);

	_, value = 123123L, false;
	fmt::printf("index={}, value={}\n", index, value);

}