package main

import {
	fmt;
	debug;
}

fn test_decimal_double() {
	fmt::printf("test_decimal_double------------------\n");

	// 基本测试
	debug::assert(123.456 == 123.456);
	debug::assert(123_456.789 == 123456.789);
	debug::assert(1_234.567_89 == 1234.56789);
	debug::assert(12_345_678.901_234 == 12345678.901234);

	// 负数测试
	debug::assert(-123.456 == -123.456);
	debug::assert(-123_456.789 == -123456.789);
	debug::assert(-1_234.567_89 == -1234.56789);
	debug::assert(-12_345_678.901_234 == -12345678.901234);


	// 零值测试
	debug::assert(0.0 == 0.0);
	debug::assert(0_0.0_0 == 0.0);
	debug::assert(-0.0 == 0.0);
	debug::assert(-0_0.0_0 == 0.0);

	// 前导零和尾随零
	debug::assert(0123.456 == 123.456);
	debug::assert(123.456000 == 123.456);
	debug::assert(0_123.456_000 == 123.456);
	debug::assert(000_123.456_000_789 == 123.456000789);

}

fn test_invalid_double_separators() {
	fmt::printf("test_invalid_double_separators------------------\n");

	// 以下是无效的分隔符使用方式，应该被注释掉
	// debug::assert(_123.456 == 123.456); // 开头有下划线
	// debug::assert(123._456 == 123.456); // 小数点前有下划线
	// debug::assert(123.456_ == 123.456); // 结尾有下划线
	// debug::assert(123.__456 == 123456.0); // 连续下划线
	// debug::assert(123.456_e10 == 1234560000000.0); // e前面有下划线
	// debug::assert(123.456e_10 == 1234560000000.0); // e后面有下划线
	// debug::assert(123.456e1_0 == 1234560000000.0); // 指数部分有下划线
	// debug::assert(123.456e+_10 == 1234560000000.0); // 指数符号后有下划线
}

fn main() {
	test_decimal_double();
	// test_invalid_double_separators();
	return;
}