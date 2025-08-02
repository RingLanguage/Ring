
package main

import {
	fmt;
	debug;
}

global {
	var int a ;
}

typedef TaskStatus = enum bool {
	Success = true;
	Failure = false;
}

typedef AccessMode = enum int {
    O_RDONLY  = 0;
    O_WRONLY  = 1;
    O_RDWR    = 2;
}

typedef Int64Const = enum int64 {
	Zero = 0L;
	One  = 1L;
	Two  = 2L;

	Min  = -9223372036854775808L;
	Max  = 9223372036854775807L;
}

typedef DoubleConst = enum double {
	A = 1.1234;
	B = 2.1234;
	C = 3.1234;
}

typedef Ring = enum string {
	Platform   = "macos";
	Name       = "Ring";
	Version    = "ring-v0.3.0-beta";
	Website    = "ring.wiki";
	Developer  = "GeneralSandman";
}


fn main() {

	fmt::println(TaskStatus::Success);
	fmt::println(TaskStatus::Failure);

	fmt::println(AccessMode::O_RDONLY);
	fmt::println(AccessMode::O_WRONLY);
	fmt::println(AccessMode::O_RDWR);


	fmt::println(Int64Const::Zero);
	fmt::println(Int64Const::One);
	fmt::println(Int64Const::Two);
	fmt::println(Int64Const::Min);
	fmt::println(Int64Const::Max);

	fmt::println(DoubleConst::A);
	fmt::println(DoubleConst::B);
	fmt::println(DoubleConst::C);

	fmt::println(Ring::Platform);
	fmt::println(Ring::Name);
	fmt::println(Ring::Version);
	fmt::println(Ring::Website);
	fmt::println(Ring::Developer);
}

