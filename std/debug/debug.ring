//
// File:    debug.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-14
//

package debug

import {

}

@native
fn assert(var bool bool_value);

@native
fn var_dump(var any... any_value);


// 这是一个专门的测试函数，不能作为标准的package 发布
typedef TestClass = class {
	var bool    Bool;
    var int     Int;
	var int64   Int64;
    var double  Double;
    var string  String;
}
fn test_return_class() -> (TestClass) {
	return TestClass{
		Bool:    true,
		Int:     2,
		Int64:   3L,
		Double:  44.44,
		String: "hello ring"
	};
}