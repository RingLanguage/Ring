package main

import {
	fmt;
}

typedef FuncType = fn();

fn test1() {
	test2();
}

fn test2() {

	// var int[] array;
	// fmt::println(cap(array));

	// 空数组报错
	// var int[] array;
	// array[10] = 1;

	// 数组越界报错
	// var int[] array;
	// array = int[] {1,2,3};
	// fmt::println(array[5]);


	// 空 匿名函数变量 报错
	var FuncType func_var;
	func_var();
}


fn main() {
	test1();
}
