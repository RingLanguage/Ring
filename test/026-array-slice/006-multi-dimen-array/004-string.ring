package main

import {
    fmt;
    debug;
}


fn test_slice_string_array() {
    fmt::printf("test_slice_string_array ------------\n");

    var string[,] local_string_array_2 = string[,]{
        string[]{"1", "2", "3"},
        string[]{"4", "5", "6"},
        string[]{"7", "8", "9"},
        string[]{"10", "11", "12"},
    };

    // 测试不同长度数组的切片
    test_string_slices(local_string_array_2);
}
fn test_string_slices(string[,] arr) {
    var int len = len(arr);
	var int start = 0;
	var int end = 0;

    // 测试 start 和 end 都存在的情况
    for start = 0; start < len; start++ {
        for end = start; end < len+1; end++ {
            fmt::printf("arr[{}:{}] = {}\n", start, end, arr[start:end]);
        }
    }

    // 测试 start 不存在的情况
    for end = 0; end < len+1; end++ {
        fmt::printf("arr[:{}] = {}\n", end, arr[:end]);
    }

    // 测试 end 不存在的情况
    for start = 0; start < len+1; start++ {
        fmt::printf("arr[{}:] = {}\n", start, arr[start:]);
    }

    // 测试 start 和 end 都不存在的情况
    fmt::printf("arr[:] = {}\n", arr[:]);
}


fn main() {
	test_slice_string_array();
}