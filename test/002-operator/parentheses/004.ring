package main

import {
	fmt;
	debug;
}

// TE: 测试混合运算符（算术、比较、逻辑）的优先级
fn main() {
	test_mixed_operators();
}

fn test_mixed_operators() {

	// 算术运算符优先级高于比较运算符
	var bool c = 2 + 3 * 4 == 14;
	var bool d = (2 + 3) * 4 == 20;
	fmt::printf("2 + 3 * 4 == 14 = {}\n", c);
	fmt::printf("(2 + 3) * 4 == 20 = {}\n", d);
	debug::assert(c == true);
	debug::assert(d == true);

	// 复杂混合表达式
	var bool e = (2 * 3 + 4) < 15 and not(5 % 2 == 0);
	fmt::printf("(2 * 3 + 4) < 15 and not(5 % 2 == 0) = {}\n", e);
	debug::assert(e == true);
}