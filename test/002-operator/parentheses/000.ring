package main

import {
	fmt;
	debug;
}


// TE: 测试括号运算符内为最基本的数据类型
fn main() {
	test_0();
}

fn test_0() {
	fmt::println((true));
	fmt::println((1));
	fmt::println((2L));
	fmt::println((3.0));
	fmt::println(("ring"));
}

