package main

import {
	fmt;
	debug;
}

// TE: 测试括号运算符指定运算顺序/优先级
fn main() {
	test_math();
	test_logical();
	test_comparison();
	test_bitwise();
	test_nested();
}



// 测试数学运算优先级
fn test_math() {
	fmt::println("Testing mathematical operations...");

	// 基本四则运算
	debug::assert((1+2)*3 == 9);
	debug::assert(1+2*3 == 7);

	debug::assert((1+2)/3 == 1);
	debug::assert(1+2/3 == 1);
	debug::assert(1+(2/3) == 1);

	// 减法和加法
	debug::assert(10-1-1 == 8);
	debug::assert(10-(1-1) == 10);

	debug::assert(10-1+1 == 10);
	debug::assert(10-(1+1) == 8);

	// 乘法和减法
	debug::assert(10-1*2 == 8);
	debug::assert((10-1)*2 == 18);

	// 多个运算数
	debug::assert((1+2+3+4+5+6+7+8+9+10)*10 == 550);
	debug::assert(1+2+3+4+5+6+7+8+9+10*10 == 145);

	// 取模运算
	debug::assert(10%3*2 == 2);
	debug::assert(10%(3*2) == 4);

	// int64 类型
	debug::assert((1000000000L * 2L) + 500000000L == 2_500_000_000L);
	debug::assert(1000000000L * (2L + 500000000L) == 500_000_002_000_000_000L);
}

// 测试逻辑运算优先级
fn test_logical() {
	fmt::println("Testing logical operations...");

	// and 和 or 的优先级
	debug::assert(true or false and false == true);
	debug::assert((true or false) and false == false);

	// not 的优先级
	debug::assert(not false and false == false);
	debug::assert((not true and true) == false);
	debug::assert(not (true and true) == false);

	// 复杂逻辑表达式
	debug::assert(true or false and not false == true);
	debug::assert((true or false) and not false == true);

	// 混合逻辑运算
	debug::assert((true and false) or (true and true) == true);
	debug::assert(true and (false or true) == true);
}

// 测试比较运算优先级
fn test_comparison() {
	fmt::println("Testing comparison operations...");

	// 比较运算与算术运算
	debug::assert(1 + 2 > 3 == false);
	// debug::assert(1 + (2 > 3) == 1); // 注意: 2>3 会被转换为 int 0

	// 多个比较运算
	debug::assert(10 > 5 == 3 > 1 == true);
	debug::assert((10 > 5) == (3 > 1) == true);

	// 比较运算与逻辑运算
	debug::assert(10 > 5 and 3 < 1 == false);
	// debug::assert(10 > (5 and 3) < 1 == false); // 注意: 5 and 3 会被转换为 int 1

	// 复杂比较表达式
	debug::assert((10 > 5) and (5 < 10) == true);
	debug::assert(10 > 5 and 5 < 10 == true);
}

// 测试位运算优先级
fn test_bitwise() {
	fmt::println("Testing bitwise operations...");

	// 位运算与算术运算
	debug::assert(1 << 1 + 1 == 4);
	debug::assert((1 << 1) + 1 == 3);

	// 连续位运算
	debug::assert(1 << 1 << 1 == 4);
	debug::assert((1 << 1) << 1 == 4);
	debug::assert(1 << (1 << 1) == 4);

	// 更多位运算
	debug::assert(1 << 2 << 2 == 16);
	debug::assert((1 << 2) << 2 == 16);
	debug::assert(1 << (2 << 2) == 256);

	// 不同位运算之间的优先级
	debug::assert(1 | 2 & 3 == 3);
	debug::assert((1 | 2) & 3 == 3);

	// 位非运算的优先级
	debug::assert(~1 & 3 == 2);
	debug::assert(~(1 & 3) == -2);

	// int64 类型位运算
	debug::assert((1L << 30) * 2L == 2147483648L);
	debug::assert(1L << (30 * 2) == 1_152_921_504_606_846_976L);
}

// 测试嵌套括号
fn test_nested() {
	fmt::println("Testing nested operations...");
	
	// 双层嵌套
	debug::assert(((1 + 2) * 3) / 4 == 2);

	// 多层嵌套
	debug::assert(((1 + 2) * (3 - 4)) / 5 == 0);

	// 混合运算嵌套
	debug::assert((10 + (5 * 2 - 3)) % 4 == 1);

	// 逻辑运算嵌套
	debug::assert((true or (false and not true)) and false == false);
	debug::assert((true or false) and (true and not false) == true);

	// 位运算嵌套
	debug::assert((1 | (2 & 3)) << 1 == 6);

	// 极端嵌套
	debug::assert((((1 + 2) * 3) - 4) / 5 == 1);
	debug::assert((((1 << 1) | 2) & 3) + 4 == 6);
}