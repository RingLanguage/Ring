package main

import {
    fmt;
    debug;
}


fn main() {
	test_slic_string();
}

fn test_slic_string() {
	fmt::printf("test_slic_string ------------\n");

	var string local_string_value_0 = "01234567890";

	test_string_slices(local_string_value_0);

}

fn test_string_slices(string string_value) {
    var int len = len(string_value);
	var int start = 0;
	var int end = 0;

    // 测试 start 和 end 都存在的情况
    for start = range 0..len {
        for end = range start..len+1 {
            fmt::printf("string_value[{}:{}] = {}\n", start, end, string_value[start:end]);
        }
    }

    // 测试 start 不存在的情况
    for end = range 0..len+1 {
        fmt::printf("string_value[:{}] = {}\n", end, string_value[:end]);
    }

    // 测试 end 不存在的情况
    for start = range 0..len+1 {
        fmt::printf("astring_valuer[{}:] = {}\n", start, string_value[start:]);
    }
}
