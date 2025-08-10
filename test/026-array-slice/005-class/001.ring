package main

import {
    fmt;
    debug;
}

typedef Job = class {
	var bool Bool;
	var int Int;
	var int64 Int64;
	var double Double;
	var string String;
}


fn test_slice_class_array() {
    fmt::printf("test_slice_class_array ------------\n");

    var Job[] local_string_array_2 = Job[]{
        Job{Bool: true, Int: 0, Int64: 0L, Double: 0.0, String: "0.0.0"},
        Job{Bool: false, Int: 1, Int64: 1L, Double: 1.1, String: "1.1.1"},
        Job{Bool: true, Int: 2, Int64: 2L, Double: 2.2, String: "2.2.2"},
        Job{Bool: false, Int: 3, Int64: 3L, Double: 3.3, String: "3.3.3"},
        Job{Bool: true, Int: 4, Int64: 4L, Double: 4.4, String: "4.4.4"},
        Job{Bool: false, Int: 5, Int64: 5L, Double: 5.5, String: "5.5.5"},
        Job{Bool: true, Int: 6, Int64: 6L, Double: 6.6, String: "6.6.6"},
        Job{Bool: false, Int: 7, Int64: 7L, Double: 7.7, String: "7.7.7"},
        Job{Bool: true, Int: 8, Int64: 8L, Double: 8.8, String: "8.8.8"},
        Job{Bool: false, Int: 9, Int64: 9L, Double: 9.9, String: "9.9.9"},
        Job{Bool: true, Int: 10, Int64: 10L, Double: 10.10, String: "10.10.10"},
    };

    // 测试不同长度数组的切片
    test_string_slices(local_string_array_2);
}
fn test_string_slices(Job[] arr) {
    var int len = len(arr);
	var int start = 0;
	var int end = 0;

    // 测试 start 和 end 都存在的情况
    for start = range 0..len {
        for end = range start..len+1 {
            fmt::printf("arr[{}:{}] = {}\n", start, end, arr[start:end]);
        }
    }

    // 测试 start 不存在的情况
    for end = range 0..len+1 {
        fmt::printf("arr[:{}] = {}\n", end, arr[:end]);
    }

    // 测试 end 不存在的情况
    for start = range 0..len+1 {
        fmt::printf("arr[{}:] = {}\n", start, arr[start:]);
    }
}


fn main() {
	test_slice_class_array();
}