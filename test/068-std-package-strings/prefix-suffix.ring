package main

import {
	fmt;
	debug;
	strings;
}

fn main() {
	test_has_prefix();
	test_trim_prefix();

	test_has_suffix();
	test_trim_suffix();
}

fn test_has_prefix() {
	fmt::printf("test_has_prefix ------------\n");

	debug::assert(strings::has_prefix("abc", "") == true);
	debug::assert(strings::has_prefix("abc", "a") == true);
	debug::assert(strings::has_prefix("abc", "ab") == true);
	debug::assert(strings::has_prefix("abc", "abc") == true);
	debug::assert(strings::has_prefix("abc", "abcd") == false);

}

fn test_trim_prefix() {
	fmt::printf("test_trim_prefix ------------\n");

	debug::assert(strings::trim_prefix("abc", "") == "abc");
	debug::assert(strings::trim_prefix("abc", "a") == "bc");
	debug::assert(strings::trim_prefix("abc", "ab") == "c");
	debug::assert(strings::trim_prefix("abc", "abc") == "");
	debug::assert(strings::trim_prefix("abc", "abcd") == "abc");
	
}

fn test_has_suffix() {
	fmt::printf("test_has_suffix ------------\n");

	debug::assert(strings::has_suffix("abc", "") == true);
	debug::assert(strings::has_suffix("abc", "c") == true);
	debug::assert(strings::has_suffix("abc", "bc") == true);
	debug::assert(strings::has_suffix("abc", "abc") == true);
	debug::assert(strings::has_suffix("abc", "1abc") == false);

}

fn test_trim_suffix() {
	fmt::printf("test_trim_suffix ------------\n");

	debug::assert(strings::trim_suffix("abc", "") == "abc");
	debug::assert(strings::trim_suffix("abc", "c") == "ab");
	debug::assert(strings::trim_suffix("abc", "bc") == "a");
	debug::assert(strings::trim_suffix("abc", "abc") == "");
	debug::assert(strings::trim_suffix("abc", "1abc") == "abc");

}