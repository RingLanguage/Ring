package main

import {
	fmt;
	debug;
}


typedef Job1 = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}


// TestDetail: 测试函数返回 bool 类型
fn main() {

	var Job1 local_job1_value_0 = Job1{
		Bool:   false,
		Int:    0,
		Int64:  0L,
		Double: 0.0,
		String: "",
	};
	
}