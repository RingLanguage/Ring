package main

import {
	fmt;
	debug;
}


fn main() {
	test_bitwise_not();
	test_bitwise_and();
	test_bitwise_or();
	test_bitwise_xor();
	test_bitwise_left_shift();
	test_bitwise_right_shift();
}

fn test_bitwise_not() {
	fmt::printf("---------------- test_bitwise_not ---------------\n");

	var int   local_int_value_0;
	var int64 local_int64_value_0;

	local_int_value_0 = ~1;
	local_int64_value_0 = ~1L;
	fmt::println(local_int_value_0);
	fmt::println(local_int64_value_0);

	local_int_value_0 = ~local_int_value_0;
	local_int64_value_0 = ~local_int64_value_0;
	fmt::println(local_int_value_0);
	fmt::println(local_int64_value_0);
}

fn test_bitwise_and() {
	fmt::printf("---------------- test_bitwise_and ---------------\n");

	fmt::println(0b1010&0b0101);
	fmt::println(0b1111&0b1111);
}

fn test_bitwise_or() {
	fmt::printf("---------------- test_bitwise_or ---------------\n");

	fmt::println(2|2);
	fmt::println(0b1010|0b0101);
	fmt::println(0x0F|0xF0);
}

fn test_bitwise_xor() {
	fmt::printf("---------------- test_bitwise_xor ---------------\n");

	fmt::println(0b1010^0b0101);
	fmt::println(0b1111^0b1111);
	fmt::println(1000^0);
}

fn test_bitwise_left_shift() {
	fmt::printf("---------------- test_bitwise_left_shift ---------------\n");

	fmt::println(0b0001<<1);
	fmt::println(0b0001<<2);
	fmt::println(0b0001<<3);
	fmt::println(0b0001<<31);
	fmt::println(0b0001<<32);

	
}

fn test_bitwise_right_shift() {
	fmt::printf("---------------- test_bitwise_right_shift ---------------\n");

    fmt::println(0b1000>>1);
	fmt::println(0b1000>>2);
	fmt::println(0b1000>>3);
	fmt::println(0b1000>>4);
	fmt::println(0b1000>>5);
}
