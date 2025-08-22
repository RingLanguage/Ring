package main

import {
	fmt;
	debug;
}

global {
	var bool 	global_bool_value;
	var int 	global_int_value;
	var int64 	global_int64_value;
	var double 	global_double_value;
	var string 	global_string_value;
}


fn main() {
	test_from_int_to_bool();
	test_from_complex_expression_to_bool();
}

fn test_from_int_to_bool() {
	fmt::printf("\ntest_from_int_to_bool-------\n");

	global_bool_value = @bool(0);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(-1);
	debug::assert(global_bool_value == true);


	global_bool_value = @bool(100);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(-100);
	debug::assert(global_bool_value == true);


	global_bool_value = @bool(2147483647);
	debug::assert(global_bool_value == true);
	
	global_bool_value = @bool(-2147483648);
	debug::assert(global_bool_value == true);
}


fn test_from_complex_expression_to_bool() {
	fmt::printf("\ntest_from_complex_expression_to_bool-------\n");

	// 数学运算
	global_bool_value = @bool(0+0);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1+1);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(2147483646+1);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(1-1);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(0*111);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1+2*3+10/10);
	debug::assert(global_bool_value == true);

	// 逻辑运算
	global_bool_value = @bool(true and true);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(true or false);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(not true);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(not not true);
	debug::assert(global_bool_value == true);

	// 关系运算
	global_bool_value = @bool(1+2 == 3);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(1+2 != 3);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1+2 < 3);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1+2 > 3);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1+2 <= 3);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(1+2 >= 3);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(1 + 2*3 + 10/10 == 8);
	debug::assert(global_bool_value == true);





	// 位运算
	global_bool_value = @bool(~0);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(~123);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(~0xFFFFFFFF);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(123 & 123);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(123 | 123);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool(123 ^ 123);
	debug::assert(global_bool_value == false);


	// 位移运算
	global_bool_value = @bool(1 >> 1);
	debug::assert(global_bool_value == false);

	global_bool_value = @bool(1 << 1);
	debug::assert(global_bool_value == true);


	// 括号运算
	global_bool_value = @bool((1+1));
	debug::assert(global_bool_value == true);

	

	// 三目条件运算符
	global_bool_value = @bool( (1+1 == 2) ? true : false);
	debug::assert(global_bool_value == true);

	global_bool_value = @bool( (1+1 > 2) ? true : false);
	debug::assert(global_bool_value == false);

	
}