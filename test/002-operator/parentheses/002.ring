package main

import {
	fmt;
	debug;
}

// TE: 测试基本算术运算符优先级（无括号）
fn main() {
	test_arithmetic_priority();
}

fn test_arithmetic_priority() {
	// 测试乘法和除法优先级高于加法和减法
	var int a = 2 + 3 * 4;
	var int b = (2 + 3) * 4;
	fmt::printf("2 + 3 * 4 = {}\n", a);
	fmt::printf("(2 + 3) * 4 = {}\n", b);
	debug::assert(a == 14);
	debug::assert(b == 20);

	var int c = 10 - 5 / 5;
	var int d = (10 - 5) / 5;
	fmt::printf("10 - 5 / 5 = {}\n", c);
	fmt::printf("(10 - 5) / 5 = {}\n", d);
	debug::assert(c == 9);
	debug::assert(d == 1);
}