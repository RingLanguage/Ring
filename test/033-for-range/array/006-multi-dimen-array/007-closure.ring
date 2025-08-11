package main

import {
	debug;
	fmt;
}

global {

}

typedef FuncType = fn();

fn main() {
	var FuncType[,] local_bool_array_0 = FuncType[,]{
		FuncType[]{
			fn() { fmt::println_string("Function 0 executed"); },
			fn() { fmt::println_string("Function 1 executed"); },
			fn() { fmt::println_string("Function 2 executed"); },
			fn() { fmt::println_string("Function 3 executed"); },
		},
		FuncType[]{
			fn() { fmt::println_string("Function 4 executed"); },
			fn() { fmt::println_string("Function 5 executed"); },
			fn() { fmt::println_string("Function 6 executed"); },
			fn() { fmt::println_string("Function 7 executed"); },
		},
		FuncType[]{
			fn() { fmt::println_string("Function 8 executed"); },
			fn() { fmt::println_string("Function 9 executed"); },
			fn() { fmt::println_string("Function 10 executed"); },
			fn() { fmt::println_string("Function 11 executed"); },
		},
		FuncType[]{
			fn() { fmt::println_string("Function 12 executed"); },
			fn() { fmt::println_string("Function 13 executed"); },
			fn() { fmt::println_string("Function 14 executed"); },
			fn() { fmt::println_string("Function 15 executed"); },
		}
	};

	var int index;
	var int index_2;
	var FuncType[] value;
	var FuncType   func_value;
	
	fmt::println_string("test case for-range --------------------");
	for _, value = range local_bool_array_0 {
		for _, func_value = range value {
			func_value();
		}
	}

	fmt::println_string("\ntest case for --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		for _, func_value = range local_bool_array_0[index] {
			func_value();
		}
	}


	fmt::println_string("\ntest case for2 --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		for index_2 = 0; index_2<len(local_bool_array_0[index]); index_2++ {
			local_bool_array_0[index,index_2]();
		}
	}


	fmt::println_string("\ntest case for3 --------------------");
	for index = 0; index<len(local_bool_array_0); index++ {
		for index_2 = 0; index_2<len(local_bool_array_0[index]); index_2++ {
			func_value = local_bool_array_0[index,index_2];
			func_value();
		}
	}

	fmt::println_string("\ntest case for-range&for --------------------");
	index = 0;
	for _, value = range local_bool_array_0 {
		for _, func_value = range value {
		    index++;
		}
	}

	fmt::println_string("\ntest case range-index --------------------");
	debug::assert(index == len(local_bool_array_0)*len(local_bool_array_0[0]));
}