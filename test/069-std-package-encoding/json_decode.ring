package main

import {
	debug;
	fmt;
	encoding;
}

typedef FuncType = fn();


typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
	var Job1    Job1Value;
	// var FuncType FuncField;

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

global {
	var Job global_job_value_0 = Job{
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
}

fn main() {
	test_json_decode_object();
	test_json_decode_array();
}

// local_job_value_0 -> json_string_0 -> local_job_value_1 -> json_string_1
// check json_string_0 == json_string_1
fn test_json_decode_object() {
	fmt::printf("test_json_decode_object ---------------\n");

	var Job local_job_value_0 = global_job_value_0;
	var Job local_job_value_1;
	var string json_string_0;
	var string json_string_1;
    var string json_decode_res;



	// 1. json_encode
	json_string_0 = encoding::json_encode(local_job_value_0);


	// 2. json_decode
	local_job_value_1, json_decode_res = encoding::json_decode(local_job_value_0, json_string_0);
    if json_decode_res != "" {
        fmt::printf("json_decode_res error: {}\n", json_decode_res);
        return;
    }
	fmt::println(local_job_value_1);


	// 3. json_encode
    json_string_1 = encoding::json_encode(local_job_value_1);


	// 4. check
    debug::assert(json_string_0 == json_string_1);
}



// local_job_value_0 -> json_string_0 -> local_job_value_1 -> json_string_1
// check json_string_0 == json_string_1
fn test_json_decode_array() {
	fmt::printf("test_json_decode_array ---------------\n");

	var Job[] local_job_array_0 = Job[]{
		global_job_value_0
	};
	var Job[] local_job_array_1;
	var string json_string_0;
	var string json_string_1;
    var string json_decode_res;



	// 1. json_encode
	json_string_0 = encoding::json_encode(local_job_array_0);


	// 2. json_decode
	local_job_array_1, json_decode_res = encoding::json_decode(local_job_array_1, json_string_0);
    if json_decode_res != "" {
        fmt::printf("json_decode_res error: {}\n", json_decode_res);
        return;
    }
	fmt::println(local_job_array_1);

	
	// 3. json_encode
    json_string_1 = encoding::json_encode(local_job_array_1);


	// 4. check
    debug::assert(json_string_0 == json_string_1);
}

