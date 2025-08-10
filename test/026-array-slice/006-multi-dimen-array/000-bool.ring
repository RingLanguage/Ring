package main

import {
    fmt;
    debug;
}


fn test_slice_bool_array() {
    fmt::printf("test_slice_bool_array ------------\n");

    var bool[,] local_bool_array_2 = bool[,]{
        bool[]{false, true, false},
        bool[]{true, false, true},
        bool[]{false, true, false},
        bool[]{true, false, true},
    };

    // 测试不同长度数组的切片
    test_bool_slices(local_bool_array_2);
}
fn test_bool_slices(bool[,] arr) {
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
}


fn main() {
	test_slice_bool_array();
}