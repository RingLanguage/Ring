package main

import {
	debug;
	fmt;
	encoding;
}

typedef FuncType = fn();


typedef Job0 = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}


typedef Job1 = Job0;


typedef Job2 = class {
	var bool[] Bool;
}

typedef Job3 = class {
	var bool Bool;
}

fn main() {
}

fn test_3() {
	var Job0 job0;
	var Job3 job3;

	job3 = job0;

}

fn test_0() {
	var Job0  local_job_value_0;
	var Job1  local_job_value_1;

	fmt::printf("local_job_value_0: {}\n", local_job_value_0);
	fmt::printf("local_job_value_1: {}\n", local_job_value_1);


	local_job_value_0 = Job1{
		Bool: true,
		Int: 1,
		Int64: 1000L,
		Double: 1.1,
		String: "Ring",
	};
	local_job_value_1 = local_job_value_0;
	fmt::printf("local_job_value_0: {}\n", local_job_value_0);
	fmt::printf("local_job_value_1: {}\n", local_job_value_1);

}

fn test_1() {
	var Job1  local_job_value_1;
	var Job2  local_job_value_2;

	fmt::printf("typeof(local_job_value_1): {}\n", reflect::typeof(local_job_value_1));
	fmt::printf("local_job_value_1: {}\n", local_job_value_1);
	fmt::printf("local_job_value_2: {}\n", local_job_value_2);

	local_job_value_2 = Job2{
		Bool: bool[]{true, false, true, false},
	};
	local_job_value_1 = local_job_value_2;
	fmt::printf("typeof(local_job_value_1): {}\n", reflect::typeof(local_job_value_1));
	fmt::printf("local_job_value_1: {}\n", local_job_value_1);
	fmt::printf("local_job_value_2: {}\n", local_job_value_2);

	
}