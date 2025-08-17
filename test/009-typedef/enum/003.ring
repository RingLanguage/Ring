
package main

import {
	fmt;
	debug;
}


typedef IntConsts = enum int {
	A = 1 + 1;
	B = 9 * 9;
}


fn main() {
	test_1();
}


fn test_1() {
	fmt::println(IntConsts::A);
	fmt::println(IntConsts::B);
}
