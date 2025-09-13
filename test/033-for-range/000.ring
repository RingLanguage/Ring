package main

import {
	debug;
	fmt;
}

global {

}


// TE: 测试 for-range 语句中，range的对象在循环过程中被修改的情况
fn main() {
	test_step();
	test_array();
}

fn test_step() {
	fmt::println("test_step --------------------");
	var int count;
	var int int_value;
	var int max;

	max = 10;

	for int_value = range 0..max {
		count++;
		if count == 5 {
			max = 20;
		}
	}

	fmt::println("count: ", count);
	debug::assert(count == 10);
}

fn test_array() {
	fmt::println("test_array --------------------");
	var int64 index;
	var bool[] local_bool_array_0 = bool[]{true, false, false};

	for index, _ = range local_bool_array_0 {
		fmt::println("index: ", index);
		if index == 1L {
			push(local_bool_array_0, true);
		}
	}

	debug::assert(index == 3L);
}
