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

// TE: 测试 json_encode/json_decode, 类中field类型为基础类型 bool/int/int64/double/string
fn main() {
	test_0();
	test_1();

}

fn test_0() {
	var Job job;
	test_json_encode_decode(job);
}

fn test_1() {
	var Job job = Job{
		Bool: true,
		Int: 2147483647,
		Int64: 9223372036854775807L,
		Double: 3.14,
		String: "hello world",
	};
	test_json_encode_decode(job);
}

fn test_json_encode_decode(Job job1) {
	var Job    job2;
	var string json_res;
	var string json_str_1;
	var string json_str_2;

	fmt::printf("test_json_encode_decode ------\n");

	fmt::printf("job1: {}\n", job1);

	json_str_1 = encoding::json_encode(job1);


	job2, json_res = encoding::json_decode(job2, json_str_1);
	if json_res != "" {
		fmt::printf("json_decode err:{}\n", json_res);
	}
	fmt::printf("job2: {}\n", job2);

	json_str_2 = encoding::json_encode(job2);


	fmt::printf("json_str_1: {}\n", json_str_1);
	fmt::printf("json_str_2: {}\n", json_str_2);

	if json_str_1 != json_str_2 {
		fmt::printf("json_str_1 != json_str_2\n");
	} else {
		fmt::printf("Test PASS: json_str_1 == json_str_2\n");
	}

	fmt::printf("test_json_encode_decode ------\n");

}