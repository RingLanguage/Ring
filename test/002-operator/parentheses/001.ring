package main

import {
	fmt;
	debug;
}


// TE: 无括号时，各运算符的优先级
fn main() {
	test_1();
}


fn test_1() {
	fmt::println(1==1 or 2==3);
	fmt::println(2*3==6 and 4*5==20);
}
