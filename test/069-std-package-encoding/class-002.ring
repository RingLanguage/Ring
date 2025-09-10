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
	var Job1    Job1Value;

	var bool[]  BoolArray;
	var int[]   IntArray;
	var int64[] Int64Array;
	var double[] DoubleArray;
	var string[] StringArray;
	var Job1[]   Job1Array;

	var bool[,]  BoolArray2;
	var int[,]   IntArray2;
	var int64[,] Int64Array2;
	var double[,] DoubleArray2;
	var string[,] StringArray2;
	var Job1[,]   Job1Array2;

}

typedef Job1 = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}

// TE: 测试 json_encode/json_decode, 类中的field 为类 和 多维数组
fn main() {
	test_0();
	test_1();

}

fn test_0() {
	fmt::printf("test_0 ------------------------------\n");
	var Job job;
	test_json_encode_decode(job);
}

fn test_1() {
	fmt::printf("test_1 ------------------------------\n");
	var Job job = Job{
		Bool: true,
		Int: 42,
		Int64: 1234567890L,
		Double: 3.14159,
		String: "Hello, Ring!",
		Job1Value: Job1{
			Bool: true,
			Int: 1,
			Int64: 2L,
			Double: 3.3,
			String:"String",
		},
		// FuncField: fn() {},

		BoolArray: bool[]{true, false, true, false, true},
		IntArray: int[]{1, 2, 3, 4, 5},
		Int64Array: int64[]{1L, 2L, 3L, 4L, 5L},
		DoubleArray: double[]{1.0, 2.0, 3.0, 4.0, 5.0},
		StringArray: string[]{"a", "b", "b", "d", "e"},
		Job1Array: Job1[]{
			Job1{
				Bool: true,
				Int: 1,
				Int64: 2L,
				Double: 3.3,
				String: "String",
			},
			Job1{
				Bool: true,
				Int: 1,
				Int64: 2L,
				Double: 3.3,
				String: "String",
			},
		},


		BoolArray2: bool[,]{
			bool[]{true, true},
			bool[]{false, false},
		},
		IntArray2: int[,]{
			int[]{1, 2},
			int[]{3, 4},
		},
		Int64Array2: int64[,]{
			int64[]{1L, 2L},
			int64[]{3L, 4L},
		},
		DoubleArray2: double[,]{
			double[]{1.1, 2.2},
			double[]{3.3, 4.4},
		},
		StringArray2: string[,]{
			string[]{"x", "y"},
			string[]{"z", "w"},
		},
		Job1Array2: Job1[,]{
			Job1[]{
				Job1{
					Bool: true,
					Int: 1,
					Int64: 2L,
					Double: 3.3,
					String: "String",
				},
				Job1{
					Bool: true,
					Int: 1,
					Int64: 2L,
					Double: 3.3,
					String: "String",
				},
			},
			Job1[]{
				Job1{
					Bool: true,
					Int: 1,
					Int64: 2L,
					Double: 3.3,
					String: "String",
				},
				Job1{
					Bool: true,
					Int: 1,
					Int64: 2L,
					Double: 3.3,
					String: "String",
				},
			},
		},
	};

	test_json_encode_decode(job);
}

fn test_json_encode_decode(Job job1) {
	var Job job2;
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