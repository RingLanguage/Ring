package main

import {
	fmt;
	debug;
}

fn main() {
	var int[] local_int_value_0 = int[]{0};
	var int64[] local_int64_value_0 = int64[]{0L};
	var double[] local_double_value_0 = double[]{0.0};
	var string[] local_string_value_0 = string[]{""};

	local_int_value_0[0] += 10;
	local_int64_value_0[0] += 20L;
	local_double_value_0[0] += 30.0;
	local_string_value_0[0] += "hello world";
	fmt::println(local_int_value_0[0]);
	fmt::println(local_int64_value_0[0]);
	fmt::println(local_double_value_0[0]);
	fmt::println(local_string_value_0[0]);
	debug::assert(local_int_value_0[0] == 10);
	debug::assert(local_int64_value_0[0] == 20L);
	debug::assert(local_double_value_0[0] == 30.0);
	debug::assert(local_string_value_0[0] == "hello world");


	local_int_value_0[0] += 10;
	local_int64_value_0[0] += 20L;
	local_double_value_0[0] += 30.0;
	local_string_value_0[0] += "hello world";
	fmt::println(local_int_value_0[0]);
	fmt::println(local_int64_value_0[0]);
	fmt::println(local_double_value_0[0]);
	fmt::println(local_string_value_0[0]);
	debug::assert(local_int_value_0[0] == 20);
	debug::assert(local_int64_value_0[0] == 40L);
	debug::assert(local_double_value_0[0] == 60.0);
	debug::assert(local_string_value_0[0] == "hello worldhello world");

}