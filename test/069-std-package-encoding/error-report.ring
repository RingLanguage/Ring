package main

import {
	debug;
	fmt;
	encoding;
}

typedef Job = class {
    var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}

fn main() {
	test_0();
}

fn test_0() {
	var string json_str;
	var string json_res;

	json_str = `{"Bool": {}}`;

	var Job job;

	job, json_res = encoding::json_decode(job, json_str);
	if json_res != "" {
		fmt::printf("json_res: {}\n", json_res);
		return;
	}
	fmt::printf("job = {}\n", job);

}