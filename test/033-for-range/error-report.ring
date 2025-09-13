package main

import {
	debug;
	fmt;
}

global {

}

// TE: 测试for range 是否检查赋值语句中 left right 类型是否匹配
fn main() {
	test_0();
	test_1();
}

fn test_0() {
	var int64 i;
	var bool value;
	var bool[] bool_array;


	bool_array = bool[]{true, false};

	for _, value = range bool_array {
		fmt::println(i, value);
	}
}


fn test_1() {
	var int64 i;
	var bool[] value;
	var bool[,] bool_array = bool[,]{
		bool[]{true, false},
		bool[]{false, true},
	};


	for _, value = range bool_array {
		fmt::println(i, value);
	}
}
