package main

import {
	fmt;
	debug;
	reflect;
}


typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}



// TE: 测试括号运算符内为常量表达式
fn main() {
	test_0();
	test_1();
	test_2();
	test_3();
}


// 没有括号运算符
fn test_0() {
	fmt::printf("\ntest_0 --------------------------\n");

	fmt::println(true);
	fmt::println(1);
	fmt::println(2L);
	fmt::println(3.0);
	fmt::println("ring");
	fmt::println(Job{});
	fmt::println(bool[]{true, false, true});
	fmt::println(int[]{1, 2, 3});
	fmt::println(int64[]{1L, 2L, 3L});
	fmt::println(double[]{1.0, 2.0, 3.0});
	fmt::println(string[]{"1", "2", "3"});
	fmt::println(Job[]{Job{}});


	fmt::println(reflect::typeof(true));
	fmt::println(reflect::typeof(1));
	fmt::println(reflect::typeof(2L));
	fmt::println(reflect::typeof(3.0));
	fmt::println(reflect::typeof("ring"));
	fmt::println(reflect::typeof(Job{}));
	fmt::println(reflect::typeof(bool[]{true, false, true}));
	fmt::println(reflect::typeof(int[]{1, 2, 3}));
	fmt::println(reflect::typeof(int64[]{1L, 2L, 3L}));
	fmt::println(reflect::typeof(double[]{1.0, 2.0, 3.0}));
	fmt::println(reflect::typeof(string[]{"1", "2", "3"}));
	fmt::println(reflect::typeof(Job[]{Job{}}));
}


// 一个括号运算符包裹的表达式
fn test_1() {
	fmt::printf("\ntest_1 --------------------------\n");

	fmt::println((true));
	fmt::println((1));
	fmt::println((2L));
	fmt::println((3.0));
	fmt::println(("ring"));
	fmt::println((Job{}));
	fmt::println((bool[]{true, false, true}));
	fmt::println((int[]{1, 2, 3}));
	fmt::println((int64[]{1L, 2L, 3L}));
	fmt::println((double[]{1.0, 2.0, 3.0}));
	fmt::println((string[]{"1", "2", "3"}));
	fmt::println((Job[]{Job{}}));


	fmt::println(reflect::typeof((true)));
	fmt::println(reflect::typeof((1)));
	fmt::println(reflect::typeof((2L)));
	fmt::println(reflect::typeof((3.0)));
	fmt::println(reflect::typeof(("ring")));
	fmt::println(reflect::typeof((Job{})));
	fmt::println(reflect::typeof((bool[]{true, false, true})));
	fmt::println(reflect::typeof((int[]{1, 2, 3})));
	fmt::println(reflect::typeof((int64[]{1L, 2L, 3L})));
	fmt::println(reflect::typeof((double[]{1.0, 2.0, 3.0})));
	fmt::println(reflect::typeof((string[]{"1", "2", "3"})));
	fmt::println(reflect::typeof((Job[]{Job{}})));
}



// 两个括号运算符包裹的表达式
fn test_2() {
	fmt::printf("\ntest_2 --------------------------\n");

	fmt::println(((true)));
	fmt::println(((1)));
	fmt::println(((2L)));
	fmt::println(((3.0)));
	fmt::println((("ring")));
	fmt::println(((Job{})));
	fmt::println(((bool[]{true, false, true})));
	fmt::println(((int[]{1, 2, 3})));
	fmt::println(((int64[]{1L, 2L, 3L})));
	fmt::println(((double[]{1.0, 2.0, 3.0})));
	fmt::println(((string[]{"1", "2", "3"})));
	fmt::println(((Job[]{Job{}})));


	fmt::println(reflect::typeof(((true))));
	fmt::println(reflect::typeof(((1))));
	fmt::println(reflect::typeof(((2L))));
	fmt::println(reflect::typeof(((3.0))));
	fmt::println(reflect::typeof((("ring"))));
	fmt::println(reflect::typeof(((Job{}))));
	fmt::println(reflect::typeof(((bool[]{true, false, true}))));
	fmt::println(reflect::typeof(((int[]{1, 2, 3}))));
	fmt::println(reflect::typeof(((int64[]{1L, 2L, 3L}))));
	fmt::println(reflect::typeof(((double[]{1.0, 2.0, 3.0}))));
	fmt::println(reflect::typeof(((string[]{"1", "2", "3"}))));
	fmt::println(reflect::typeof(((Job[]{Job{}}))));
}


// 三个括号运算符包裹的表达式
fn test_3() {
	fmt::printf("\ntest_3 --------------------------\n");
	fmt::println((((true))));
	fmt::println((((1))));
	fmt::println((((2L))));
	fmt::println((((3.0))));
	fmt::println((((("ring")))));
	fmt::println((((Job{}))));
	fmt::println((((bool[]{true, false, true}))));
	fmt::println((((int[]{1, 2, 3}))));
	fmt::println((((int64[]{1L, 2L, 3L}))));
	fmt::println((((double[]{1.0, 2.0, 3.0}))));
	fmt::println((((string[]{"1", "2", "3"}))));
	fmt::println((((Job[]{Job{}}))));

	fmt::println(reflect::typeof(((((true))))));
	fmt::println(reflect::typeof(((((1))))));
	fmt::println(reflect::typeof(((((2L))))));
	fmt::println(reflect::typeof(((((3.0))))));
	fmt::println(reflect::typeof((((("ring"))))));
	fmt::println(reflect::typeof(((((Job{}))))));
	fmt::println(reflect::typeof(((((bool[]{true, false, true}))))));
	fmt::println(reflect::typeof(((((int[]{1, 2, 3}))))));
	fmt::println(reflect::typeof(((((int64[]{1L, 2L, 3L}))))));
	fmt::println(reflect::typeof(((((double[]{1.0, 2.0, 3.0}))))));
	fmt::println(reflect::typeof(((((string[]{"1", "2", "3"}))))));
	fmt::println(reflect::typeof(((((Job[]{Job{}}))))));
}

