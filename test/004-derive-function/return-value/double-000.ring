package main

import {
	fmt;
	debug;
}


// TestDetail: 测试函数返回 bool 类型
fn main() {
	fmt::println(return_1_double_0());
	fmt::println(return_1_double_1());

	fmt::println(return_2_double_0());
	fmt::println(return_2_double_1());

	fmt::println(return_3_double_0());
	fmt::println(return_3_double_1());

	fmt::println(return_4_double_0());
	fmt::println(return_4_double_1());

	fmt::println(return_5_double_0());
	fmt::println(return_5_double_1());

	fmt::println(return_6_double_0());
	fmt::println(return_6_double_1());

	fmt::println(return_7_double_0());
	fmt::println(return_7_double_1());

	fmt::println(return_8_double_0());
	fmt::println(return_8_double_1());


}




fn return_1_double_0() -> (double) {
	return 0.0;
}
fn return_1_double_1() -> (double) {
	return 1.0;
}



fn return_2_double_0() -> (double,double) {
	return 0.0, 1.0;
}
fn return_2_double_1() -> (double,double) {
	return 1.0, 0.0;
}



fn return_3_double_0() -> (double,double,double) {
	return 0.0, 1.0, 0.0;
}
fn return_3_double_1() -> (double,double,double) {
	return 1.0, 0.0, 1.0;
}



fn return_4_double_0() -> (double,double,double,double) {
	return 0.0, 1.0, 0.0, 1.0;
}
fn return_4_double_1() -> (double,double,double,double) {
	return 1.0, 0.0, 1.0, 0.0;
}



fn return_5_double_0() -> (double,double,double,double,double) {
	return 0.0, 1.0, 0.0, 1.0, 0.0;
}
fn return_5_double_1() -> (double,double,double,double,double) {
	return 1.0, 0.0, 1.0, 0.0, 1.0;
}



fn return_6_double_0() -> (double,double,double,double,double,double) {
	return 0.0, 1.0, 0.0, 1.0, 0.0, 1.0;
}
fn return_6_double_1() -> (double,double,double,double,double,double) {
	return 1.0, 0.0, 1.0, 0.0, 1.0, 0.0;
}



fn return_7_double_0() -> (double,double,double,double,double,double,double) {
	return 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0;
}
fn return_7_double_1() -> (double,double,double,double,double,double,double) {
	return 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0;
}



fn return_8_double_0() -> (double,double,double,double,double,double,double,double) {
	return 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0;
}
fn return_8_double_1() -> (double,double,double,double,double,double,double,double) {
	return 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0;
}