package main

import {
	fmt;
	debug;
}

fn test_binary() {
	fmt::printf("test_binary------------------\n");

	debug::assert(0b11 == 3);
	debug::assert(0b11L == 3L);
	debug::assert(0b101010 == 42);
	debug::assert(0b101010L == 42L);
	debug::assert(0b11111111 == 255);
	debug::assert(0b11111111L == 255L);
	debug::assert(-0b1010 == -10);
	debug::assert(-0b1010L == -10L);
	debug::assert(0b00001111 == 15);
	debug::assert(0b00001111L == 15L);
    debug::assert(0b111111111111111111111111111111111111111111111111111111111111111L == 9223372036854775807L);
    debug::assert(-0b1000000000000000000000000000000000000000000000000000000000000000L == -9223372036854775808L);
}

fn test_octal() {
	fmt::printf("test_octal------------------\n");

	debug::assert(0o120 == 80);
	debug::assert(0o120L == 80L);
	debug::assert(0o777 == 511);
	debug::assert(0o777L == 511L);
	debug::assert(0o1234 == 668);
	debug::assert(0o1234L == 668L);
	debug::assert(-0o12 == -10);
	debug::assert(-0o12L == -10L);
	debug::assert(0o00012 == 10);
	debug::assert(0o00012L == 10L);
    debug::assert(0o777777777777777777777L == 9223372036854775807L);
    debug::assert(-0o1000000000000000000000L == -9223372036854775808L);
}

fn test_hex() {
	fmt::printf("test_hex------------------\n");

	debug::assert(0xFF == 255);
	debug::assert(0xFFL == 255L);
	debug::assert(0xa3 == 163);
	debug::assert(0xA3 == 163);
	debug::assert(0xa3L == 163L);
	debug::assert(0xffaabb == 16755387);
	debug::assert(0xFFAABB == 16755387);
	debug::assert(0xffaabbL == 16755387L);
	debug::assert(-0xA == -10);
	debug::assert(-0xAL == -10L);
	debug::assert(0x000A == 10);
	debug::assert(0x000AL == 10L);
	debug::assert(0x7FFFFFFFFFFFFFFFL == 9223372036854775807L);
	debug::assert(0x8000000000000000L == 9223372036854775808L);
	debug::assert(0xFFFFFFFFFFFFFFFFL == 18446744073709551615L);
	fmt::println("0xFFFFFFFFFFFFFFFFL = ", 0xFFFFFFFFFFFFFFFFL);
	fmt::println("18446744073709551615L = ", 18446744073709551615L);
}

fn test_decimal() {
	fmt::printf("test_decimal------------------\n");

	fmt::printf("012340={}\n", 012340);
	fmt::printf("012340L={}\n", 012340L);
	debug::assert(012340 ==  12340);
	debug::assert(012340L == 12340L);
	debug::assert(12340 == 12340);
	debug::assert(12340L == 12340L);
	debug::assert(0 == 0);
	debug::assert(0L == 0L);
	debug::assert(-42 == -42);
	debug::assert(-42L == -42L);
	debug::assert(987654321 == 987654321);
	debug::assert(987654321L == 987654321L);
	debug::assert(123456789012345L == 123456789012345L);
	debug::assert(9223372036854775807L == 9223372036854775807L);
	debug::assert(-9223372036854775808L == -9223372036854775808L);
}

fn main() {
	test_binary();
	test_octal();
	test_hex();
	test_decimal();
	return;
}