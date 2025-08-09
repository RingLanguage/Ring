package main

import {
	fmt;
	debug;
}


// TestDetail: derive函数传递bool类型参数
// TestDetail: 分别传递1-8个bool类型参数
fn main() {

	pass_1_bool(false);
	pass_1_bool(true);

	pass_2_bool(false, false);
	pass_2_bool(true,  true);

	pass_3_bool(false, false, false);
	pass_3_bool(true,  true,  true);

	pass_4_bool(false, false, false, false);
	pass_4_bool(true,  true,  true,  true);

	pass_5_bool(false, false, false, false, false);
	pass_5_bool(true,  true,  true,  true,  true);

	pass_6_bool(false, false, false, false, false, false);
	pass_6_bool(true,  true,  true,  true,  true,  true);

	pass_7_bool(false, false, false, false, false, false, false);
	pass_7_bool(true,  true,  true,  true,  true,  true,  true);

	pass_8_bool(false, false, false, false, false, false, false, false);
	pass_8_bool(true,  true,  true,  true,  true,  true,  true,  true);
}





fn pass_1_bool(bool a) {
	fmt::println(a);
}

fn pass_2_bool(bool a, bool b) {
	fmt::println(a, b);
}

fn pass_3_bool(bool a, bool b, bool c) {
    fmt::println(a, b, c);
}

fn pass_4_bool(bool a, bool b, bool c, bool d) {
    fmt::println(a, b, c, d);
}

fn pass_5_bool(bool a, bool b, bool c, bool d, bool e) {
    fmt::println(a, b, c, d, e);
}

fn pass_6_bool(bool a, bool b, bool c, bool d, bool e, bool f) {
    fmt::println(a, b, c, d, e, f);
}

fn pass_7_bool(bool a, bool b, bool c, bool d, bool e, bool f, bool g) {
    fmt::println(a, b, c, d, e, f, g);
}

fn pass_8_bool(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) {
    fmt::println(a, b, c, d, e, f, g, h);
}

