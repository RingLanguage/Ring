package main

import {
	fmt;
	debug;
}

fn main() {
	var int local_int_value_0;
	var int64 local_int64_value_0;
	var double local_double_value_0;
	var string local_string_value_0;

	local_int_value_0 += 10;
	local_int64_value_0 += 20L;
	local_double_value_0 += 30.0;
	local_string_value_0 += "hello world";
	fmt::println(local_int_value_0);
	fmt::println(local_int64_value_0);
	fmt::println(local_double_value_0);
	fmt::println(local_string_value_0);
	debug::assert(local_int_value_0 == 10);
	debug::assert(local_int64_value_0 == 20L);
	debug::assert(local_double_value_0 == 30.0);
	debug::assert(local_string_value_0 == "hello world");


	local_int_value_0 += 10;
	local_int64_value_0 += 20L;
	local_double_value_0 += 30.0;
	local_string_value_0 += "hello world";
	fmt::println(local_int_value_0);
	fmt::println(local_int64_value_0);
	fmt::println(local_double_value_0);
	fmt::println(local_string_value_0);
	debug::assert(local_int_value_0 == 20);
	debug::assert(local_int64_value_0 == 40L);
	debug::assert(local_double_value_0 == 60.0);
	debug::assert(local_string_value_0 == "hello worldhello world");

}