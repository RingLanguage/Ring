package main

import {
	debug;
	fmt;
}

global {

}


fn main() {
	test_step_int_increase();
	test_step_int_decrease();
}

// 增续
fn test_step_int_increase() {
	fmt::printf("test_step_int_increase-----------\n");

	var int count;
	var double double_value;

	// 标准用法，开区间
	count = 0;
	for double_value = range 0.0..10.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 10);
	// 指定步长，开区间
	count = 0;
	for double_value = range 0.0..10.0:1.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 10);
	// 指定步长，开区间
	count = 0;
	for double_value = range 0.0..10.0:2.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 5);
	// 指定步长，开区间
	count = 0;
	for double_value = range 0.0..10.0:3.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 4);


	// -------------------------

	
	// 标准用法，闭区间
	count = 0;
	for double_value = range 0.0..=10.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 11);
	// 指定步长，闭区间
	count = 0;
	for double_value = range 0.0..=10.0:1.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 11);
	// 指定步长，闭区间
	count = 0;
	for double_value = range 0.0..=10.0:2.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 6);
	// 指定步长，闭区间
	count = 0;
	for double_value = range 0.0..=10.0:3.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 4);
}

// 降序
fn test_step_int_decrease() {
	fmt::printf("test_step_int_decrease-----------\n");

	var int count;
	var double double_value;

	// 指定步长，开区间
	count = 0;
	for double_value = range 10.0..0.0:-1.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 10);
	// 指定步长，开区间
	count = 0;
	for double_value = range 10.0..0.0:-2.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 5);
	// 指定步长，开区间
	count = 0;
	for double_value = range 10.0..0.0:-3.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 4);


	// -------------------------

	
	// 指定步长，闭区间
	count = 0;
	for double_value = range 10.0..=0.0:-1.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 11);
	count = 0;
	for double_value = range 10.0..=0.0:-2.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 6);
	// 指定步长，闭区间
	count = 0;
	for double_value = range 10.0..=0.0:-3.0 {
		fmt::println("double_value: ", double_value);
		count++;
	}
	debug::assert(count == 4);

}