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

fn main() {
    test_0();
}

fn test_0() {
    var string json_decode_res;

    var Job local_job_value_0;
	var bool[] local_bool_array_1;

	var string json_string = `
    [true, false]
    `;

    local_bool_array_1, json_decode_res = encoding::json_decode(local_bool_array_1, json_string);
    if json_decode_res != "" {
        fmt::printf("error: {}\n", json_decode_res);
        return;
    }

    fmt::printf("{}\n", local_bool_array_1);
}



fn test_1() {
	var Job local_job_value_0 = Job{
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

	var string json_string;
	var Job local_job_value_1;
    var string err_msg;

	// 1. println
	// fmt::println(local_job_value_0);
	// fmt::println(local_job_value_1);

	// 3. json_encode
	json_string = encoding::json_encode(local_job_value_0);
	fmt::println(json_string);

	// 5. json_decode
	local_job_value_1, err_msg = encoding::json_decode(local_job_value_0, json_string);
    if err_msg != "" {
        fmt::printf("error: {}\n", err_msg);
        return;
    }
    fmt::printf("parse success-------------\n");
	// fmt::println(local_job_value_0);
	fmt::println(local_job_value_1);
}

fn test_00() {


	var Job local_job_value_0;
	var string json_string = `
	{
    "Bool": true,
    "BoolArray": [
        true,
        false,
        true,
        false,
        true
    ],
    "BoolArray2": [
        [
            true,
            true
        ],
        [
            false,
            false
        ]
    ],
    "Double": 3.14159,
    "DoubleArray": [
        1.0,
        2.0,
        3.0,
        4.0,
        5.0
    ],
    "DoubleArray2": [
        [
            1.1,
            2.2
        ],
        [
            3.3,
            4.4
        ]
    ],
    "Int": 42,
    "Int64": 1234567890,
    "Int64Array": [
        1,
        2,
        3,
        4,
        5
    ],
    "Int64Array2": [
        [
            1,
            2
        ],
        [
            3,
            4
        ]
    ],
    "IntArray": [
        1,
        2,
        3,
        4,
        5
    ],
    "IntArray2": [
        [
            1,
            2
        ],
        [
            3,
            4
        ]
    ],
    "Job1Array": [
        {
            "Bool": true,
            "Double": 3.3,
            "Int": 1,
            "Int64": 2,
            "String": "String"
        },
        {
            "Bool": true,
            "Double": 3.3,
            "Int": 1,
            "Int64": 2,
            "String": "String"
        }
    ],
    "Job1Array2": [
        [
            {
                "Bool": true,
                "Double": 3.3,
                "Int": 1,
                "Int64": 2,
                "String": "String"
            },
            {
                "Bool": true,
                "Double": 3.3,
                "Int": 1,
                "Int64": 2,
                "String": "String"
            }
        ],
        [
            {
                "Bool": true,
                "Double": 3.3,
                "Int": 1,
                "Int64": 2,
                "String": "String"
            },
            {
                "Bool": true,
                "Double": 3.3,
                "Int": 1,
                "Int64": 2,
                "String": "String"
            }
        ]
    ],
    "Job1Value": {
        "Bool": true,
        "Double": 3.3,
        "Int": 1,
        "Int64": 2,
        "String": "String"
    },
    "String": "Hello, Ring!",
    "StringArray": [
        "a",
        "b",
        "b",
        "d",
        "e"
    ],
    "StringArray2": [
        [
            "x",
            "y"
        ],
        [
            "z",
            "w"
        ]
    ]
}
	`;

	// fmt::println_pointer(local_job_value_0);
    var string err_msg;

	local_job_value_0, err_msg = encoding::json_decode(local_job_value_0, json_string);

	// fmt::println_pointer(local_job_value_0);

	fmt::println(local_job_value_0);
}
