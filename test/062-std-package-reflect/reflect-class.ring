//
// File:    std-reflect-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-10-07
//

package main

import {
	fmt;
  	debug;
	reflect;
}

typedef Job = class {
    var bool    Bool;
    var int     Int;
    var int64   Int64;
    var double  Double;
    var string  String;
}

global {
	
	var Job    global_job_class_value_1;
}

fn main() {
	var Job    job_class_value = Job{
		Bool:   true,
		Int:    123,
		Int64:  1234567890123456789L,
		Double: 3.141592,
		String: "Hello, Ring!"
	};
	var int64 field_index;
	var string field_name;


	// test 1 get_obj_fields
	fmt::println(reflect::get_obj_fields(job_class_value));

	// test 2 get_obj_field_type
	fmt::println(reflect::get_obj_field_type(job_class_value, "Bool"));
	fmt::println(reflect::get_obj_field_type(job_class_value, "Int"));
	fmt::println(reflect::get_obj_field_type(job_class_value, "Int64"));
	fmt::println(reflect::get_obj_field_type(job_class_value, "Double"));
	fmt::println(reflect::get_obj_field_type(job_class_value, "String"));
	fmt::println(reflect::get_obj_field_type(job_class_value, "xxx"));

	// test 3 get_obj_field_value
	fmt::println(reflect::get_obj_field_value(job_class_value, "Bool"));
	fmt::println(reflect::get_obj_field_value(job_class_value, "Int"));
	fmt::println(reflect::get_obj_field_value(job_class_value, "Int64"));
	fmt::println(reflect::get_obj_field_value(job_class_value, "Double"));
	fmt::println(reflect::get_obj_field_value(job_class_value, "String"));
	fmt::println(reflect::get_obj_field_value(job_class_value, "xxx"));

	//
	fmt::println(job_class_value);
	test_access_class(job_class_value);

	// test 4 set_obj_field_value
	reflect::set_obj_field_value(job_class_value, "Bool", false);
	reflect::set_obj_field_value(job_class_value, "Int", 456);
	reflect::set_obj_field_value(job_class_value, "Int64", 9876543210L);
	reflect::set_obj_field_value(job_class_value, "Double", 6.283185);
	reflect::set_obj_field_value(job_class_value, "String", "Hello, Ring! Again!");
	reflect::set_obj_field_value(job_class_value, "xxx", 1234567890L);

	// 
	fmt::println(job_class_value);
	test_access_class(job_class_value);

}


fn test_access_class(Job job) {
	fmt::println("\ntest_access_class-------------");

	var int64 field_index;
	var string field_name;
	var string field_type;
	var string field_value;

	for field_index, field_name = range reflect::get_obj_fields(job) {
	    fmt::printf("Index={} Name={} Type={} Value={}\n",
		    field_index, 
			field_name,
			reflect::get_obj_field_type(job, field_name),
			reflect::get_obj_field_value(job, field_name)
		);
	}

	fmt::println("-------------------------------");

}