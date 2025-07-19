package main

import {
	fmt;
	debug;
}

fn test_binary_separator() {
	fmt::printf("test_binary_separator------------------\n");

	// 有效的二进制分隔符
	debug::assert(0b10_10 == 10);
	debug::assert(0b10_10L == 10L);
	debug::assert(0b1111_1111 == 255);
	debug::assert(0b1111_1111L == 255L);
	debug::assert(-0b10_10 == -10);
	debug::assert(-0b10_10L == -10L);
	debug::assert(0b1010_1010_1010 == 2730);
	debug::assert(0b1010_1010_1010L == 2730L);
	debug::assert(0b0000_1111 == 15);
	debug::assert(0b0000_1111L == 15L);

	// 无效的二进制分隔符（应该会编译错误，但我们注释掉以避免测试失败）
	// debug::assert(0b_1010 == 10); // 开头有_
	// debug::assert(0b1010_ == 10); // 结尾有_
	// debug::assert(0b10__10 == 10); // 连续_
}

fn test_octal_separator() {
	fmt::printf("test_octal_separator------------------\n");

	// 有效的八进制分隔符
	debug::assert(0o12_34 == 668);
	debug::assert(0o12_34L == 668L);
	debug::assert(0o7_77 == 511);
	debug::assert(0o7_77L == 511L);
	debug::assert(-0o1_2 == -10);
	debug::assert(-0o1_2L == -10L);
	debug::assert(0o12_34_56 == 42798);
	debug::assert(0o12_34_56L == 42798L);
	debug::assert(0o000_12 == 10);
	debug::assert(0o000_12L == 10L);

	// 无效的八进制分隔符
	// debug::assert(0o_123 == 83); // 开头有_
	// debug::assert(0o123_ == 83); // 结尾有_
	// debug::assert(0o1__23 == 83); // 连续_
}

fn test_hex_separator() {
	fmt::printf("test_hex_separator------------------\n");

	// 有效的十六进制分隔符
	debug::assert(0xFF_AA == 65450);
	debug::assert(0xFF_AAL == 65450L);
	debug::assert(0xA_3 == 163);
	debug::assert(0xA_3L == 163L);
	debug::assert(-0xA_A == -170);
	debug::assert(-0xA_AL == -170L);
	debug::assert(0xFF_AA_BB == 16755387);
	debug::assert(0xFF_AA_BBL == 16755387L);
	debug::assert(0x000_A == 10);
	debug::assert(0x000_AL == 10L);
	debug::assert(0x7FFF_FFFF == 2147483647);
	debug::assert(0x7FFF_FFFFL == 2147483647L);

	// 无效的十六进制分隔符
	// debug::assert(0x_FF == 255); // 开头有_
	// debug::assert(0xFF_ == 255); // 结尾有_
	// debug::assert(0xF__F == 255); // 连续_
}

fn test_decimal_separator() {
	fmt::printf("test_decimal_separator------------------\n");

	// 有效的十进制分隔符
	debug::assert(1_234 == 1234);
	debug::assert(1_234L == 1234L);
	debug::assert(123_456 == 123456);
	debug::assert(123_456L == 123456L);
	debug::assert(-1_234 == -1234);
	debug::assert(-1_234L == -1234L);
	debug::assert(1_000_000 == 1000000);
	debug::assert(1_000_000L == 1000000L);
	debug::assert(9_223_372_036_854_775_807L == 9223372036854775807L);
	debug::assert(-9_223_372_036_854_775_808L == -9223372036854775808L);
	debug::assert(12_345_678 == 12345678);
	debug::assert(12_345_678L == 12345678L);

	// 无效的十进制分隔符
	// debug::assert(_1234 == 1234); // 开头有_
	// debug::assert(1234_ == 1234); // 结尾有_
	// debug::assert(1__234 == 1234); // 连续_
}

fn main() {
	test_binary_separator();
	test_octal_separator();
	test_hex_separator();
	test_decimal_separator();
	return;
}