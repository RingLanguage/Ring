package main

import {
	fmt;
	debug;
}

// TE: 测试int64类型的运算符优先级和括号影响
fn main() {
	test_int64_priority();
}

fn test_int64_priority() {
	// 测试int64类型的算术运算优先级
	var int64 a = 1000000000L + 2000000000L * 3L;
	var int64 b = (1000000000L + 2000000000L) * 3L;
	fmt::printf("1000000000L + 2000000000L * 3L = {}\n", a);
	fmt::printf("(1000000000L + 2000000000L) * 3L = {}\n", b);
	debug::assert(a == 7000000000L);
	debug::assert(b == 9000000000L);

	// 测试大数值下的括号效果
	var int64 c = (9223372036854775807L - 100L) / 2L;
	fmt::printf("(9223372036854775807L - 100L) / 2L = {}\n", c);
	debug::assert(c == 4611686018427387853L);

	// 测试int64与int混合运算中的优先级
	var int64 d = 5L * @int64(100 + 200);
	fmt::printf("5L * @int64(100 + 200) = {}\n", d);
	debug::assert(d == 1500L);
}