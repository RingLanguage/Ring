package main

import {
	os;
}

fn main() {
	// os::exit(25);
	test1(26);
}

fn test1(int a) -> (string) {
	os::exit(a);
}