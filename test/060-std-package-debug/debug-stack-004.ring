//
// File:    std-debug-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-24
//

package main

import {
	runtime;
}


fn main() {
	test0();
}


// ----------------------

fn test0() {
	test_two_bool(false, true);
}

fn test_two_bool(bool bool_value_0, bool bool_value_1) {
	test_two_int(1, 2);
}

fn test_two_int(int int_value_0, int int_value_1) {
	test_two_double(1.2, 2.3);
}

fn test_two_double(double double_value_0, double double_value_1) {
	test_two_string("", "");
}

fn test_two_string(string string_value_0, string string_value_1) {
	test_any(false, 1, 1.2, "a");
}

fn test_any(bool bool_value_0, int int_value_0, double double_value_0, string string_value_0) {
	runtime::print_call_stack();
}