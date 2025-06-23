package main

import {
    fmt;
    debug;
}


fn main() {
	test_slic_string();
	test_slice_bool_array();
	test_slice_int_array();
}

fn test_slic_string() {
	fmt::printf("test_slic_string ------------\n");

	var string local_string_value_0 = "01234567890";

	fmt::println(local_string_value_0);
	fmt::println(local_string_value_0[0:0]);
	fmt::println(local_string_value_0[0:1]);
	fmt::println(local_string_value_0[0:2]);
	fmt::println(local_string_value_0[0:3]);
	fmt::println(local_string_value_0[0:4]);
	fmt::println(local_string_value_0[0:5]);
	fmt::println(local_string_value_0[0:6]);
	fmt::println(local_string_value_0[0:7]);
	fmt::println(local_string_value_0[0:8]);
	fmt::println(local_string_value_0[0:9]);
	fmt::println(local_string_value_0[0:10]);

	fmt::println(local_string_value_0[1:0]);
	fmt::println(local_string_value_0[1:1]);
	fmt::println(local_string_value_0[1:2]);
	fmt::println(local_string_value_0[1:3]);
	fmt::println(local_string_value_0[1:4]);
	fmt::println(local_string_value_0[1:5]);
	fmt::println(local_string_value_0[1:6]);
	fmt::println(local_string_value_0[1:7]);
	fmt::println(local_string_value_0[1:8]);
	fmt::println(local_string_value_0[1:9]);
	fmt::println(local_string_value_0[1:10]);


	fmt::println(local_string_value_0[:5]);
	fmt::println(local_string_value_0[5:]);


	fmt::println(local_string_value_0[0:3] == "012");

}

fn test_slice_bool_array() {
	fmt::printf("test_slice_bool_array ------------\n");

	var bool[] local_bool_array_0 = bool[]{true, false, true, false, true};


	fmt::println(local_bool_array_0[0:0]);
	fmt::println(local_bool_array_0[0:1]);
	fmt::println(local_bool_array_0[0:2]);
	fmt::println(local_bool_array_0[0:3]);
	fmt::println(local_bool_array_0[0:4]);
	fmt::println(local_bool_array_0[0:5]);
	fmt::println(local_bool_array_0[0:6]);


	fmt::println(local_bool_array_0[1:0]);
	fmt::println(local_bool_array_0[1:1]);
	fmt::println(local_bool_array_0[1:2]);
	fmt::println(local_bool_array_0[1:3]);
	fmt::println(local_bool_array_0[1:4]);
	fmt::println(local_bool_array_0[1:5]);
	fmt::println(local_bool_array_0[1:6]);

}

fn test_slice_int_array() {
	fmt::printf("test_slice_int_array ------------\n");

	var int[] local_int_array_0 = int[]{0, 1, 2, 3, 4, 5, 6};


	fmt::println(local_int_array_0[0:0]);
	fmt::println(local_int_array_0[0:1]);
	fmt::println(local_int_array_0[0:2]);
	fmt::println(local_int_array_0[0:3]);
	fmt::println(local_int_array_0[0:4]);
	fmt::println(local_int_array_0[0:5]);
	fmt::println(local_int_array_0[0:6]);
	fmt::println(local_int_array_0[0:7]);


	fmt::println(local_int_array_0[1:0]);
	fmt::println(local_int_array_0[1:1]);
	fmt::println(local_int_array_0[1:2]);
	fmt::println(local_int_array_0[1:3]);
	fmt::println(local_int_array_0[1:4]);
	fmt::println(local_int_array_0[1:5]);
	fmt::println(local_int_array_0[1:6]);
	fmt::println(local_int_array_0[1:7]);

}