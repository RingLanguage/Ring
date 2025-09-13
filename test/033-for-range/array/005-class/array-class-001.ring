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

// TE: 对于 class 数组的 for-range 测试，range的对象是个常量表达式
fn main() {
	var int64 index;
	var Job value;
	
	fmt::println_string("test case for-range --------------------");
	for index, value = range Job[]{
		Job{Bool: false, Int: 0, Int64: 0L, Double: 0.0, String: "Job0"},
		Job{Bool: true, Int: 1, Int64: 1L, Double: 1.1, String: "Job1"},
		Job{Bool: false, Int: 2, Int64: 2L, Double: 2.2, String: "Job2"},
		Job{Bool: true, Int: 3, Int64: 3L, Double: 3.3, String: "Job3"},
		Job{Bool: false, Int: 4, Int64: 4L, Double: 4.4, String: "Job4"},
	} {
		fmt::println(value);
	}
	debug::assert(index == 4L);

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0L;
	for _, value = range Job[]{
		Job{Bool: false, Int: 0, Int64: 0L, Double: 0.0, String: "Job0"},
		Job{Bool: true, Int: 1, Int64: 1L, Double: 1.1, String: "Job1"},
		Job{Bool: false, Int: 2, Int64: 2L, Double: 2.2, String: "Job2"},
		Job{Bool: true, Int: 3, Int64: 3L, Double: 3.3, String: "Job3"},
		Job{Bool: false, Int: 4, Int64: 4L, Double: 4.4, String: "Job4"},
	} {
		index++;
	}
	debug::assert(index == 5L);
}