//
// File:    debug-stack-003.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-12-13
//

package main

import {
	fmt;
	runtime;
}


fn main() {
	fmt::println("fn main()");
	test0();
}

fn test0() {
	fmt::println("fn test0()");
	test1(false);
}

fn test1(bool bool_value) {
	fmt::println("fn test1()");
	test2(1);
}

fn test2(int int_value) {
	fmt::println("fn test2()");
	test3(1.2);
}

fn test3(double double_value) {
	fmt::println("fn test3()");
	test4("a");
}

fn test4(string string_value) {
	fmt::println("fn test4()");
	runtime::print_call_stack();
}
