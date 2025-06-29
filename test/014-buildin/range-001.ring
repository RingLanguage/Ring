package main

import {
    fmt;
    debug;
    runtime;
	reflect;
}

fn main() {
	test_range_bool_array_literal();
}


fn test_range_bool_array_literal() {
	fmt::printf("test_range_bool_array_literal -------------\n");
	
	var bool bool_tmp;

	for(bool_tmp = range bool[]{false, true, false, true}) {
		fmt::printf("value = {}\n", bool_tmp);
	}
}


fn test_1() {
	var int index;

	for(index = 0; index < 10; index ++) {
		fmt::printf("index = {}\n", index);
	}
}