package main

import {
	debug;
	fmt;
	encoding;
}

typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}

// TE: 测试 json_encode/json_decode, 数组中的元素为类
fn main() {
	test_job_array();
}

fn test_job_array() {
	var Job[] array0;
	var Job[] array1 = Job[]{
		Job{Bool: true, Int: 1, Int64: 1L, Double: 1.1, String: "1"},
		Job{Bool: false, Int: 2, Int64: 2L, Double: 2.2, String: "2"},
		Job{Bool: true, Int: 3, Int64: 3L, Double: 3.3, String: "3"},
	};

	test_json_encode_decode_array_job(array0);
	test_json_encode_decode_array_job(array1);
}

fn test_json_encode_decode_array_job(Job[] array1) {
	var Job[] array2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode_array_job ------\n");
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

	
}