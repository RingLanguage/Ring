package main

import {
    fmt;
    debug;
}


fn test_slice_int64_array() {
    fmt::printf("test_slice_int64_array ------------\n");

    var int64[] local_int64_array_2 = int64[]{1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 11L, 12L, 13L, 14L, 15L, 16L};

    // 测试不同长度数组的切片
    test_int64_slices(local_int64_array_2);
}
fn test_int64_slices(int64[] arr) {
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
	test_slice_int64_array();
}