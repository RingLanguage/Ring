package main

import {
    fmt;
    debug;
}


fn test_slice_double_array() {
    fmt::printf("test_slice_double_array ------------\n");

    var double[] local_double_array_2 = double[]{1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16};

    // 测试不同长度数组的切片
    test_double_slices(local_double_array_2);
}
fn test_double_slices(double[] arr) {
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
	test_slice_double_array();
}