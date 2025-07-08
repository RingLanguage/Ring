//
// File:    std-debug-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-24
//

package main

import {
	fmt;
	runtime;
}

typedef Job = class {
    var bool    Bool;
    var int     Int;
    var double  Double;
    var string  String;

	fn test1() {
		global_tool_value.test1();
    }
}


typedef Tool = class {
    var bool    Bool;
    var int     Int;
    var double  Double;
    var string  String;

    fn test1() {
    }

}


global {
	var bool   global_bool_value;
	var int    global_int_value;
	var double global_double_value;
	var string global_string_value;
	var Job    global_job_value;
	var Tool   global_tool_value;
}

// 编译报错，类中不能使用 global变量
/*
./test/999-bug-list/class-use-global-var.ring:21:0: 
|               global_tool_value.test1();
| ^......
|
|Err: use undeclared identifier `global_tool_value`; E:200000.
|Tip: definite variable `global_tool_value` like: `var bool|int|double|string global_tool_value;` before use it.
|RingDebug: fix_ast.cpp:1121
*/
fn main() {
	global_job_value.test1();

}
