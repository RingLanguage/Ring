package main

import {
	debug;
	fmt;
	encoding;
}

// TE: 测试 json_encode/json_decode, 基础类型的数组 bool/int/int64/double/string
fn main() {
	test_bool_array();
	test_int_array();
	test_int64_array();
	test_double_array();
	test_string_array();
}

fn test_bool_array() {
	fmt::println("test_bool_array--------");

	var bool[] array0;
	var bool[] array1 = bool[]{false, true, false};

	test_json_encode_decode_array_bool(array0);
	test_json_encode_decode_array_bool(array1);
}

fn test_int_array() {
	fmt::println("test_int_array--------");

	var int[] array0;
	var int[] array1 = int[]{1, 2, 3};

	test_json_encode_decode_array_int(array0);
	test_json_encode_decode_array_int(array1);
}

fn test_int64_array() {
	fmt::println("test_int64_array--------");

	var int64[] array0;
	var int64[] array1 = int64[]{1L, 2L, 3L};

	test_json_encode_decode_array_int64(array0);
	test_json_encode_decode_array_int64(array1);
}

fn test_double_array() {
	fmt::println("test_double_array--------");

	var double[] array0;
	var double[] array1 = double[]{1.1, 2.2, 3.3};

	test_json_encode_decode_array_double(array0);
	test_json_encode_decode_array_double(array1);
}

fn test_string_array() {
	fmt::println("test_string_array--------");

	var string[] array0;
	var string[] array1 = string[]{"1", "2", "3"};

	test_json_encode_decode_array_string(array0);
	test_json_encode_decode_array_string(array1);
}


fn test_json_encode_decode_array_bool(bool[] array1) {
	var bool[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_bool ------\n");

	json_str_1 = encoding::json_encode(array1);

	array2, json_res = encoding::json_decode(array2, json_str_1);
	if json_res != "" {
		fmt::printf("json_res: %s", json_res);
	}

	json_str_2 = encoding::json_encode(array2);

	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);
	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS\n");
	}

	fmt::printf("test_json_encode_decode_array_bool ------\n");
}


fn test_json_encode_decode_array_int(int[] array1) {
	var int[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_int ------\n");

	json_str_1 = encoding::json_encode(array1);

	array2, json_res = encoding::json_decode(array2, json_str_1);
	if json_res != "" {
		fmt::printf("json_res: %s", json_res);
	}

	json_str_2 = encoding::json_encode(array2);

	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);
	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS\n");
	}

	fmt::printf("test_json_encode_decode_array_int ------\n");
}

fn test_json_encode_decode_array_int64(int64[] array1) {
	var int64[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_int64 ------\n");

	json_str_1 = encoding::json_encode(array1);

	array2, json_res = encoding::json_decode(array2, json_str_1);
	if json_res != "" {
		fmt::printf("json_res: %s", json_res);
	}

	json_str_2 = encoding::json_encode(array2);

	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);
	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS\n");
	}

	fmt::printf("test_json_encode_decode_array_int64 ------\n");
}

fn test_json_encode_decode_array_double(double[] array1) {
	var double[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_double ------\n");

	json_str_1 = encoding::json_encode(array1);

	array2, json_res = encoding::json_decode(array2, json_str_1);
	if json_res != "" {
		fmt::printf("json_res: %s", json_res);
	}

	json_str_2 = encoding::json_encode(array2);

	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);
	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS\n");
	}

	fmt::printf("test_json_encode_decode_array_double ------\n");
}

fn test_json_encode_decode_array_string(string[] array1) {
	var string[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_string ------\n");

	json_str_1 = encoding::json_encode(array1);

	array2, json_res = encoding::json_decode(array2, json_str_1);
	if json_res != "" {
		fmt::printf("json_res: %s", json_res);
	}

	json_str_2 = encoding::json_encode(array2);

	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);
	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS\n");
	}

	fmt::printf("test_json_encode_decode_array_string ------\n");
}
