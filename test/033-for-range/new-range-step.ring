package main

import {
	debug;
	fmt;
}

global {

}


fn main() {

	var int int_value;
	var int64 int64_value;
	var double double_value;
	

	for int_value = range 0..10:2 {
		fmt::println("Current value: ", int_value);
	}
	for int64_value = range 0..=10L {
		fmt::println("int64_value: ", int64_value);
	}
	for double_value = range 0.0..=10.0:5.0 {
		fmt::println("double_value: ", double_value);
	}

	

}
