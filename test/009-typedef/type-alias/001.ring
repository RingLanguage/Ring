
package main

import {
	fmt;
	debug;
}

global {
}


typedef BoolArray    = bool[];
typedef IntArray     = int[];
typedef Int64Array   = int64[];
typedef DoubleArray  = double[];
typedef StringArray  = string[];


fn main() {
	var BoolArray bool_array;
	var IntArray int_array;
	var Int64Array int64_array;
	var DoubleArray double_array;
	var StringArray string_array;


	bool_array = bool[]{true, false, true, false};
	int_array = int[]{1, 2, 3, 4, 5};
	int64_array = int64[]{10000000000L, 20000000000L, 30000000000L};
	double_array = double[]{1.1, 2.2, 3.3, 4.4};
	string_array = string[]{"Hello", "World", "Ring", "Language"};

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
	formatBoolArray(bool[]{true, false, true, false});
	formatIntArray(int[]{1, 2, 3, 4, 5});
	formatInt64Array(int64[]{10000000000L, 20000000000L, 30000000000L});
	formatDoubleArray(double[]{1.1, 2.2, 3.3, 4.4});
	formatStringArray(string[]{"Hello", "World", "Ring", "Language"});
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