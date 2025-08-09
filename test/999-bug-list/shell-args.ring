//
// File:    string-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2022-05-19
//


package main

import {
	fmt;
	debug;
}

fn main(string[] args) {

    fmt::println(len(args));
	fmt::println(args);

	var string arg;
	for _, arg = range args {
		fmt::println(arg);
	}
}