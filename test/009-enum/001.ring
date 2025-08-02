
package main

import {
	fmt;
	debug;
}

global {
	var int a ;
}

typedef BoolConst = enum bool {
	True  = true;
	False = false;
}

typedef IntConst = enum int {
	IntMin_Bin = 0b10000000000000000000000000000000;
	IntMin_Oct = 0o20000000000;
	IntMin_Hex = 0x80000000;
	IntMin_Dec = -2147483648;

	IntMax_Bin = 0b01111111111111111111111111111111;
	IntMax_Oct = 0o17777777777;
	IntMax_Hex = 0x7FFFFFFF;
	IntMax_Dec = 2147483647;
}

typedef Int64Const = enum int64 {
	Int64Min_Bin = 0b1000000000000000000000000000000000000000000000000000000000000000L;
	Int64Min_Oct = 0o1000000000000000000000L;
	Int64Min_Hex = 0x8000000000000000L;
	Int64Min_Dec = -9223372036854775808L;

	Int64Max_Bin = 0b0111111111111111111111111111111111111111111111111111111111111111L;
	Int64Max_Oct = 0o777777777777777777777L;
	Int64Max_Hex = 0x7FFFFFFFFFFFFFFFL;
	Int64Max_Dec = 9223372036854775807L;
}

typedef MathConst = enum double {
	// pi
	Pi = 3.14159265358979323846;

	// e
	E = 2.71828182845904523536;
}

typedef Ring = enum string {
	Platform   = "macos";
	Name       = "Ring";
	Version    = "ring-v0.3.0-beta";
	Website    = "ring.wiki";
	Developer  = "GeneralSandman";
}

fn main() {
	test_bool();
	test_int();
	test_int64();
	test_double();
	test_string();
}

fn test_bool() {
	fmt::printf("\ntest_bool ------------\n");
	fmt::println(BoolConst::True);
	fmt::println(BoolConst::False);

	debug::assert(BoolConst::True == true);
	debug::assert(BoolConst::False == false);
}

fn test_int() {
	fmt::printf("\ntest_int ------------\n");
	fmt::println(IntConst::IntMin_Bin);
	fmt::println(IntConst::IntMin_Oct);
	fmt::println(IntConst::IntMin_Hex);
	fmt::println(IntConst::IntMin_Dec);

	fmt::println(IntConst::IntMax_Bin);
	fmt::println(IntConst::IntMax_Oct);
	fmt::println(IntConst::IntMax_Hex);
	fmt::println(IntConst::IntMax_Dec);

	debug::assert(IntConst::IntMin_Bin == -2147483648);
	debug::assert(IntConst::IntMin_Oct == -2147483648);
	debug::assert(IntConst::IntMin_Hex == -2147483648);
	debug::assert(IntConst::IntMin_Dec == -2147483648);

	debug::assert(IntConst::IntMax_Bin == 2147483647);
	debug::assert(IntConst::IntMax_Oct == 2147483647);
	debug::assert(IntConst::IntMax_Hex == 2147483647);
	debug::assert(IntConst::IntMax_Dec == 2147483647);

}

fn test_int64() {
	fmt::printf("\ntest_int64 ------------\n");
	fmt::println(Int64Const::Int64Min_Bin);
	fmt::println(Int64Const::Int64Min_Oct);
	fmt::println(Int64Const::Int64Min_Hex);
	fmt::println(Int64Const::Int64Min_Dec);

	fmt::println(Int64Const::Int64Max_Bin);
	fmt::println(Int64Const::Int64Max_Oct);
	fmt::println(Int64Const::Int64Max_Hex);
	fmt::println(Int64Const::Int64Max_Dec);

	debug::assert(Int64Const::Int64Min_Bin == -9223372036854775808L);
	debug::assert(Int64Const::Int64Min_Oct == -9223372036854775808L);
	debug::assert(Int64Const::Int64Min_Hex == -9223372036854775808L);
	debug::assert(Int64Const::Int64Min_Dec == -9223372036854775808L);

	debug::assert(Int64Const::Int64Max_Bin == 9223372036854775807L);
	debug::assert(Int64Const::Int64Max_Oct == 9223372036854775807L);
	debug::assert(Int64Const::Int64Max_Hex == 9223372036854775807L);
	debug::assert(Int64Const::Int64Max_Dec == 9223372036854775807L);
}

fn test_double() {
	fmt::printf("\ntest_double ------------\n");
	fmt::println(MathConst::Pi);
	fmt::println(MathConst::E);

	debug::assert(MathConst::Pi == 3.14159265358979323846);
	debug::assert(MathConst::E == 2.71828182845904523536);
}

fn test_string() {
	fmt::printf("\ntest_string ------------\n");
	fmt::println(Ring::Platform);
	fmt::println(Ring::Name);
	fmt::println(Ring::Version);
	fmt::println(Ring::Website);
	fmt::println(Ring::Developer);

	debug::assert(Ring::Platform == "macos");
	debug::assert(Ring::Name == "Ring");
	debug::assert(Ring::Version == "ring-v0.3.0-beta");
	debug::assert(Ring::Website == "ring.wiki");
	debug::assert(Ring::Developer == "GeneralSandman");
}

