package main

import {
	fmt;
	debug;
}


// TestDetail: derive函数传递int类型参数
// TestDetail: 分别传递1-8个int类型参数
fn main() {

	pass_1_int(0);
	pass_1_int(1);

	pass_2_int(0, 0);
	pass_2_int(1, 1);

	pass_3_int(0, 0, 0);
	pass_3_int(1, 1, 1);

	pass_4_int(0, 0, 0, 0);
	pass_4_int(1, 1, 1, 1);

	pass_5_int(0, 0, 0, 0, 0);
	pass_5_int(1, 1, 1, 1, 1);

	pass_6_int(0, 0, 0, 0, 0, 0);
	pass_6_int(1, 1, 1, 1, 1, 1);

	pass_7_int(0, 0, 0, 0, 0, 0, 0);
	pass_7_int(1, 1, 1, 1, 1, 1, 1);

	pass_8_int(0, 0, 0, 0, 0, 0, 0, 0);
	pass_8_int(1, 1, 1, 1, 1, 1, 1, 1);
}





fn pass_1_int(int a) {
	fmt::println(a);
}

fn pass_2_int(int a, int b) {
	fmt::println(a, b);
}

fn pass_3_int(int a, int b, int c) {
    fmt::println(a, b, c);
}

fn pass_4_int(int a, int b, int c, int d) {
    fmt::println(a, b, c, d);
}

fn pass_5_int(int a, int b, int c, int d, int e) {
    fmt::println(a, b, c, d, e);
}

fn pass_6_int(int a, int b, int c, int d, int e, int f) {
    fmt::println(a, b, c, d, e, f);
}

fn pass_7_int(int a, int b, int c, int d, int e, int f, int g) {
    fmt::println(a, b, c, d, e, f, g);
}

fn pass_8_int(int a, int b, int c, int d, int e, int f, int g, int h) {
    fmt::println(a, b, c, d, e, f, g, h);
}

