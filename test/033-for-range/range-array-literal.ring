package main

import {
	debug;
	fmt;
}

global {

}

typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}

typedef FuncType = fn();


fn main() {
	test_0();
	test_1();
}

// 测试通过
// range 的操作数 是数组常量
fn test_0() {
	fmt::printf("test_0-----------\n");
	var int64 		index; // 这里 int int64 都可以
	var bool 		bool_value;
	var int 		int_value;
	var int64 		int64_value;
	var double 		double_value;
	var string 		string_value;
	var Job 		job_value;
	var FuncType 	func_value;
	var int[]   	int_array_value;

	
	for index, bool_value = range bool[]{true, false, true, false} {
		fmt::println(index, bool_value);
	}
	for index, int_value = range int[]{1, 2, 3, 4} {
		fmt::println(index, int_value);
	}
	for index, int64_value = range int64[]{100L, 200L, 300L, 400L} {
		fmt::println(index, int64_value);
	}
	for index, double_value = range double[]{1.1, 2.2, 3.3, 4.4} {
		fmt::println(index, double_value);
	}
	for index, string_value = range string[]{"a", "b", "c", "d"} {
		fmt::println(index, string_value);
	}
	for index, job_value = range Job[]{
		Job{Bool: true, Int: 1, Int64: 100L, Double: 1.1, String: "a"},
		Job{Bool: false, Int: 2, Int64: 200L, Double: 2.2, String: "b"},
		Job{Bool: true, Int: 3, Int64: 300L, Double: 3.3, String: "c"},
		Job{Bool: false, Int: 4, Int64: 400L, Double: 4.4, String: "d"},
	} {
		fmt::println(index, job_value);
	}
	for index, func_value = range FuncType[]{
		fn() {
			fmt::println("Function 1 executed");
		},
		fn() {
			fmt::println("Function 2 executed");
		},
		fn() {
			fmt::println("Function 3 executed");
		},
		fn() {
			fmt::println("Function 4 executed");
		},
	} {
		fmt::printf("Executing function at index local_func_array_0[{}]\n", index);
		func_value(); // 调用匿名函数
	}
	for index, int_array_value = range int[,]{
		int[]{1, 2, 3},
		int[]{4, 5, 6},
		int[]{7, 8, 9},
	} {
		fmt::println(index, int_array_value);
	}
}

// 测试通过
// range 的操作数 是局部数组变量
fn test_1() {
	fmt::printf("test_1-----------\n");

	var bool[] local_bool_array_0 = bool[]{true, false, true, false};
	var int[] local_int_array_0 = int[]{1, 2, 3, 4};
	var int64[] local_int64_array_0 = int64[]{100L, 200L, 300L, 400L};
	var double[] local_double_array_0 = double[]{1.1, 2.2, 3.3, 4.4};
	var string[] local_string_array_0 = string[]{"a", "b", "c", "d"};
	var Job[] local_job_array_0 = Job[]{
		Job{Bool: true, Int: 1, Int64: 100L, Double: 1.1, String: "a"},
		Job{Bool: false, Int: 2, Int64: 200L, Double: 2.2, String: "b"},
		Job{Bool: true, Int: 3, Int64: 300L, Double: 3.3, String: "c"},
		Job{Bool: false, Int: 4, Int64: 400L, Double: 4.4, String: "d"},
	};
	var FuncType[] local_func_array_0 = FuncType[]{
		fn() {
			fmt::println("Function 1 executed");
		},
		fn() {
			fmt::println("Function 2 executed");
		},
		fn() {
			fmt::println("Function 3 executed");
		},
		fn() {
			fmt::println("Function 4 executed");
		},
	};

	var int[,] local_int_array_1 = int[,]{
		int[]{1, 2, 3},
		int[]{4, 5, 6},
		int[]{7, 8, 9},
	};


	var int64  		index; // 这里 int int64 都可以
	var bool 		bool_value;
	var int 		int_value;
	var int64 		int64_value;
	var double 		double_value;
	var string 		string_value;
	var Job 		job_value;
	var FuncType 	func_value;
	var int[]   	int_array_value;

	for index, bool_value = range local_bool_array_0 {
		fmt::println(index, bool_value);
	}
	for index, int_value = range local_int_array_0 {
		fmt::println(index, int_value);
	}
	for index, int64_value = range local_int64_array_0 {
		fmt::println(index, int64_value);
	}
	for index, double_value = range local_double_array_0 {
		fmt::println(index, double_value);
	}
	for index, string_value = range local_string_array_0 {
		fmt::println(index, string_value);
	}
	for index, job_value = range local_job_array_0 {
		fmt::println(index, job_value);
	}
	for index, func_value = range local_func_array_0 {
		fmt::printf("Executing function at index local_func_array_0[{}]\n", index);
		func_value(); // 调用匿名函数
	}
	for index, int_array_value = range local_int_array_1 {
		fmt::println(index, int_array_value);
	}

}