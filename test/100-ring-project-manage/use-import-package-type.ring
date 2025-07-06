package main

import {
	debug;
	fmt;
	encoding;
	reflect;
}


// TestDetail: 测试使用第三方包的 类型
fn main() {
	test_0();
	test_1();
}

fn test_0() {
	fmt::println("test_0------------------\n");

	var debug::TestClass local_test_class_value_0;
	local_test_class_value_0 = debug::TestClass{
	};
	fmt::println(local_test_class_value_0);

	
	local_test_class_value_0 = debug::test_return_class();
	fmt::println(local_test_class_value_0);
	fmt::println(local_test_class_value_0.Bool);
	fmt::println(local_test_class_value_0.Int);
	fmt::println(local_test_class_value_0.Int64);
	fmt::println(local_test_class_value_0.Double);
	fmt::println(local_test_class_value_0.String);


	local_test_class_value_0.Bool = false;
	local_test_class_value_0.Int = 222;
	local_test_class_value_0.Int64 = 333L;
	local_test_class_value_0.Double = 444.0;
	local_test_class_value_0.String = "ring-compiler";
	fmt::println(local_test_class_value_0);
	fmt::println(local_test_class_value_0.Bool);
	fmt::println(local_test_class_value_0.Int);
	fmt::println(local_test_class_value_0.Int64);
	fmt::println(local_test_class_value_0.Double);
	fmt::println(local_test_class_value_0.String);


}

fn test_1() {
	fmt::println("test_1------------------\n");

	var debug::TestClass[] local_test_class_array_0 = debug::TestClass[]{
		debug::TestClass{
			Bool:    true,
			Int:     2,
			Int64:   3L,
			Double:  44.44,
			String: "hello ring"
		},
		debug::TestClass{
			Bool:    true,
			Int:     2,
			Int64:   3L,
			Double:  44.44,
			String: "hello ring"
		},
	};

	fmt::println(local_test_class_array_0);
}