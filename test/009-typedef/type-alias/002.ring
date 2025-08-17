
package main

import {
	fmt;
	debug;
}

global {
}


typedef BoolArray    = bool[,];
typedef IntArray     = int[,];
typedef Int64Array   = int64[,];
typedef DoubleArray  = double[,];
typedef StringArray  = string[,];


fn main() {
	var BoolArray bool_array;
	var IntArray int_array;
	var Int64Array int64_array;
	var DoubleArray double_array;
	var StringArray string_array;


	bool_array = bool[,]{
		bool[]{true, false, true, false},
		bool[]{false, true, false, true},
	};
	int_array = int[,]{
		int[]{1, 2, 3, 4, 5},
		int[]{6, 7, 8, 9, 10},
	};
	int64_array = int64[,]{
		int64[]{1000L, 2000L, 3000L, 4000L, 5000L},
		int64[]{6000L, 7000L, 8000L, 9000L, 10000L},
	};
	double_array = double[,]{
		double[]{1.1, 2.2, 3.3, 4.4},
		double[]{5.5, 6.6, 7.7, 8.8},
	};
	string_array = string[,]{
		string[]{"Ring", "Language", "Test", "Go"},
		string[]{"Array", "Type", "Alias", "Example"},
	};

	fmt::println("-----------------\n");
	fmt::println(bool_array);
	fmt::println(int_array);
	fmt::println(int64_array);
	fmt::println(double_array);
	fmt::println(string_array);


	fmt::println("-----------------\n");
	formatBoolArray(bool_array);
	formatIntArray(int_array);
	formatInt64Array(int64_array);
	formatDoubleArray(double_array);
	formatStringArray(string_array);




	fmt::println("-----------------\n");
	formatBoolArray(bool[,]{
		bool[]{true, false, true, false},
		bool[]{false, true, false, true},
	});
	formatIntArray(int[,]{
		int[]{1, 2, 3, 4, 5},
		int[]{6, 7, 8, 9, 10},
	});
	formatInt64Array(int64[,]{
		int64[]{1000L, 2000L, 3000L, 4000L, 5000L},
		int64[]{6000L, 7000L, 8000L, 9000L, 10000L},
	});
	formatDoubleArray(double[,]{
		double[]{1.1, 2.2, 3.3, 4.4},
		double[]{5.5, 6.6, 7.7, 8.8},
	});
	formatStringArray(string[,]{
		string[]{"Ring", "Language", "Test", "Go"},
		string[]{"Array", "Type", "Alias", "Example"},
	});
}

fn formatBoolArray(BoolArray arr) {
	fmt::println("Bool Array: ", arr);
}
fn formatIntArray(IntArray arr) {
	fmt::println("Int Array: ", arr);
}
fn formatInt64Array(Int64Array arr) {
	fmt::println("Int64 Array: ", arr);
}
fn formatDoubleArray(DoubleArray arr) {
	fmt::println("Double Array: ", arr);
}
fn formatStringArray(StringArray arr) {
	fmt::println("String Array: ", arr);
}