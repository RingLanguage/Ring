package main

import {
	debug;
	fmt;
}

global {

}

typedef Job = class {
	var bool Bool;
	var int Int;
	var int64 Int64;
	var double Double;
	var string String;
}

fn main() {
	var Job[,] local_bool_array_0 = Job[,]{
		Job[]{
			Job{Bool: true, Int: 0, Int64: 0L, Double: 0.0, String: "Job0"},
			Job{Bool: false, Int: 1, Int64: 1L, Double: 1.1, String: "Job1"},
			Job{Bool: true, Int: 2, Int64: 2L, Double: 2.2, String: "Job2"},
			Job{Bool: false, Int: 3, Int64: 3L, Double: 3.3, String: "Job3"},
		},
		Job[]{
			Job{Bool: true, Int: 4, Int64: 4L, Double: 4.4, String: "Job4"},
			Job{Bool: false, Int: 5, Int64: 5L, Double: 5.5, String: "Job5"},
			Job{Bool: true, Int: 6, Int64: 6L, Double: 6.6, String: "Job6"},
			Job{Bool: false, Int: 7, Int64: 7L, Double: 7.7, String: "Job7"},
		},
		Job[]{
			Job{Bool: true, Int: 8, Int64: 8L, Double: 8.8, String: "Job8"},
			Job{Bool: false, Int: 9, Int64: 9L, Double: 9.9, String: "Job9"},
			Job{Bool: true, Int: 10, Int64: 10L, Double: 10.10, String: "Job10"},
			Job{Bool: false, Int: 11, Int64: 11L, Double: 11.11, String: "Job11"},
		},
		Job[]{
			Job{Bool: true, Int: 12, Int64: 12L, Double: 12.12, String: "Job12"},
			Job{Bool: false, Int: 13, Int64: 13L, Double: 13.13, String: "Job13"},
			Job{Bool: true, Int: 14, Int64: 14L, Double: 14.14, String: "Job14"},
			Job{Bool: false, Int: 15, Int64: 15L, Double: 15.15, String: "Job15"},
		}
	};

	var int index;
	var Job[] value;
	
	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		fmt::println(value);
	}

	fmt::println_string("\ntest case for --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		fmt::println(local_bool_array_0[index]);
	}

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0;
	for _, value = range local_bool_array_0 {
		index++;
	}

	fmt::println_string("\ntest case range-index --------------------");
	debug::assert(index == len(local_bool_array_0));
}