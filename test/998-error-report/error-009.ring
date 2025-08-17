package main


import {
	fmt;
}

fn main() {
	fmt::println(0b1_1);
	fmt::println(0b11);
	// --
	fmt::println(0b_11);
}