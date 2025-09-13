package main

import {
	fmt;
	debug;
}

// TE: 测试double类型的运算符优先级和括号影响
fn main() {
	test_double_priority();
}

fn test_double_priority() {
	// 测试double类型的算术运算优先级
	var double a = 1.5 + 2.5 * 3.0;
	var double b = (1.5 + 2.5) * 3.0;
	fmt::printf("1.5 + 2.5 * 3.0 = {}\n", a);
	fmt::printf("(1.5 + 2.5) * 3.0 = {}\n", b);
	debug::assert(a == 9.0);
	debug::assert(b == 12.0);

	// 测试浮点数除法中的括号效果
	var double c = 10.0 / (2.0 + 3.0);
	var double d = 10.0 / 2.0 + 3.0;
	fmt::printf("10.0 / (2.0 + 3.0) = {}\n", c);
	fmt::printf("10.0 / 2.0 + 3.0 = {}\n", d);
	debug::assert(c == 2.0);
	debug::assert(d == 8.0);

	// 测试复杂浮点表达式
	var double e = (2.5 * 3.5 + 1.5) / (4.0 - 1.5);
	fmt::printf("(2.5 * 3.5 + 1.5) / (4.0 - 1.5) = {}\n", e);
	debug::assert(e == 4.1); // 2.5*3.5=8.75+1.5=10.25 / 2.5=4.1
}