
package main

import {
	fmt;
	debug;
}

typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}


fn test_concat_0() {
	fmt::printf("test_concat_0--------\n");

	var string local_string_value_0 = "a";


	local_string_value_0 = local_string_value_0 + "b";
	fmt::printf("local_string_value_0={}\n", local_string_value_0);
	debug::assert(local_string_value_0 == "ab");


	local_string_value_0 = local_string_value_0 + "cdefghijklmnopqrstuvwxyz";
	fmt::printf("local_string_value_0={}\n", local_string_value_0);
	debug::assert(local_string_value_0 == "abcdefghijklmnopqrstuvwxyz");
}

fn test_concat_1() {
	fmt::printf("test_concat_1--------\n");

	var string local_string_value_0 = "a";

	local_string_value_0 += "b";
	fmt::printf("local_string_value_0={}\n", local_string_value_0);
	debug::assert(local_string_value_0 == "ab");



	local_string_value_0 += "cd" + "ef";
	fmt::printf("local_string_value_0={}\n", local_string_value_0);
	debug::assert(local_string_value_0 == "abcdef");


	local_string_value_0 += "ghijklmnopqrstuvwxyz";
	fmt::printf("local_string_value_0={}\n", local_string_value_0);
	debug::assert(local_string_value_0 == "abcdefghijklmnopqrstuvwxyz");
}



fn test_concat_2() {
	fmt::printf("test_concat_2--------\n");

	var string[] local_string_array_0 = string[]{"", ""};

	local_string_array_0[0] = "a" + "b";
	local_string_array_0[1] = "c" + "d";
	fmt::printf("{}\n", local_string_array_0);


	local_string_array_0[0] = local_string_array_0[0] + "--";
	local_string_array_0[1] = local_string_array_0[1] + "--";
	fmt::printf("{}\n", local_string_array_0);
}


fn test_concat_3() {
	fmt::printf("test_concat_3--------\n");

	var string[] local_string_array_0 = string[]{"", ""};

	local_string_array_0[0] += "a" + "b";
	local_string_array_0[1] += "c" + "d";
	fmt::printf("{}\n", local_string_array_0);


	local_string_array_0[0] += "--";
	local_string_array_0[1] += "--";
	fmt::printf("{}\n", local_string_array_0);
}


fn test_concat_4() {
	fmt::printf("test_concat_4--------\n");

	var string[,] local_string_array_1 = string[,]{
		string[]{"1", "2"},
		string[]{"3", "4"},
	};

	fmt::printf("{}\n", local_string_array_1);


	local_string_array_1[0,0] = local_string_array_1[0,0] + "a";
	local_string_array_1[0,1] = local_string_array_1[0,1] + "a";
	local_string_array_1[1,0] = local_string_array_1[1,0] + "a";
	local_string_array_1[1,1] = local_string_array_1[1,1] + "a";
	fmt::printf("{}\n", local_string_array_1);


	local_string_array_1[0,0] = local_string_array_1[0,0] + "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[0,1] = local_string_array_1[0,1] + "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[1,0] = local_string_array_1[1,0] + "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[1,1] = local_string_array_1[1,1] + "bcdefghijklmnopqrstuvwxyz";
	fmt::printf("{}\n", local_string_array_1);
}

fn test_concat_5() {
	fmt::printf("test_concat_5--------\n");

	var string[,] local_string_array_1 = string[,]{
		string[]{"1", "2"},
		string[]{"3", "4"},
	};

	fmt::printf("{}\n", local_string_array_1);


	local_string_array_1[0,0] += "a";
	local_string_array_1[0,1] += "a";
	local_string_array_1[1,0] += "a";
	local_string_array_1[1,1] += "a";
	fmt::printf("{}\n", local_string_array_1);


	local_string_array_1[0,0] += "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[0,1] += "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[1,0] += "bcdefghijklmnopqrstuvwxyz";
	local_string_array_1[1,1] += "bcdefghijklmnopqrstuvwxyz";
	fmt::printf("{}\n", local_string_array_1);
}


fn test_concat_6() {
	fmt::printf("test_concat_6--------\n");

	var Job local_job_value_0;

	local_job_value_0.String = "a" + "b";
	fmt::printf("local_job_value_0.String={}\n", local_job_value_0.String);
	debug::assert(local_job_value_0.String == "ab");

	local_job_value_0.String += "c" + "d";
	fmt::printf("local_job_value_0.String={}\n", local_job_value_0.String);
	debug::assert(local_job_value_0.String == "abcd");


	local_job_value_0.String += "efghijklmnopqrstuvwxyz";
	fmt::printf("local_job_value_0.String={}\n", local_job_value_0.String);
	debug::assert(local_job_value_0.String == "abcdefghijklmnopqrstuvwxyz");
}


fn test_concat_7() {
	fmt::printf("test_concat_7--------\n");

	var Job[] local_job_array_0 = new Job[2];

	local_job_array_0[0].String = "a" + "b";
	fmt::printf("local_job_array_0[0].String={}\n", local_job_array_0[0].String);
	debug::assert(local_job_array_0[0].String == "ab");

	local_job_array_0[0].String += "c" + "d";
	fmt::printf("local_job_array_0[0].String={}\n", local_job_array_0[0].String);
	debug::assert(local_job_array_0[0].String == "abcd");

	local_job_array_0[0].String += "efghijklmnopqrstuvwxyz";
	fmt::printf("local_job_array_0[0].String={}\n", local_job_array_0[0].String);
	debug::assert(local_job_array_0[0].String == "abcdefghijklmnopqrstuvwxyz");

	fmt::printf("local_job_array_0={}\n", local_job_array_0);
}

fn test_concat_8() {
    fmt::printf("test_concat_8--------\n");

	var Job[,] local_job_array_0 = new Job[2,2];

	local_job_array_0[1,1].String = "a" + "b";
	fmt::printf("local_job_array_0[1,1].String={}\n", local_job_array_0[1,1].String);
	debug::assert(local_job_array_0[1,1].String == "ab");

	local_job_array_0[1,1].String += "c" + "d";
	fmt::printf("local_job_array_0[1,1].String={}\n", local_job_array_0[1,1].String);
	debug::assert(local_job_array_0[1,1].String == "abcd");

	local_job_array_0[1,1].String += "efghijklmnopqrstuvwxyz";
	fmt::printf("local_job_array_0[1,1].String={}\n", local_job_array_0[1,1].String);
	debug::assert(local_job_array_0[1,1].String == "abcdefghijklmnopqrstuvwxyz");

	fmt::printf("local_job_array_0={}\n", local_job_array_0);

}


// TE: 充分测试字符串拼接运算符
fn main() {

	test_concat_0();
	test_concat_1();
	test_concat_2();
	test_concat_3();
	test_concat_4();
	test_concat_5();

	test_concat_6();
	test_concat_7();
	test_concat_8();

}