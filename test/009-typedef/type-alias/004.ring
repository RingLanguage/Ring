package main

import {
	debug;
	fmt;
	encoding;
}

typedef FuncType0 = fn();


typedef FuncType1 = FuncType0;


typedef FuncType2 = fn() -> (string);

fn main() {
	test_0();
}

fn test_0() {
	var FuncType0 func0;
	var FuncType1 func1;
	var FuncType2 func2;


	func0 = fn() {
		fmt::println("func0");
	};
	func1 = func0;
	func2 = fn() -> (string) {
		fmt::println("func2");
		return "func2";
	};

	func0();
	func1();
	func2();

	// all right

	// 匿名函数的类型不匹配，报错
	// func2 = func0;
	// func2 = func1;
	
}