package main

import {
	fmt;
	debug;
}

// TE: 测试括号对算术运算结果的影响
fn main() {
	test_nested_parentheses();
}

fn test_nested_parentheses() {
	// 测试多层嵌套括号
	var int a = 2 * (3 + 4 * (5 - 2));
	var int b = (2 * 3 + 4) * 5 - 2;
	fmt::printf("2 * (3 + 4 * (5 - 2)) = {}\n", a);
	fmt::printf("(2 * 3 + 4) * 5 - 2 = {}\n", b);
	debug::assert(a == 30);
	debug::assert(b == 48);

	// 测试复杂表达式中的括号效果
	var int c = (1 + 2) * (3 + 4) / (5 - 2);
	var int d = 1 + 2 * 3 + 4 / (5 - 2);
	fmt::printf("(1 + 2) * (3 + 4) / (5 - 2) = {}\n", c);
	fmt::printf("1 + 2 * 3 + 4 / (5 - 2) = {}\n", d);
	debug::assert(c == 7);
	debug::assert(d == 8); // 注意：4/3会被截断为1
}