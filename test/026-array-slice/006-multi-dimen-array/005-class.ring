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

    var Job[,] local_string_array_2 = Job[,]{
        Job[]{
            Job{Bool: true, Int: 0, Int64: 0L, Double: 0.0, String: "0.0.0"},
            Job{Bool: false, Int: 1, Int64: 1L, Double: 1.1, String: "1.1.1"},
        },
        Job[]{
            Job{Bool: true, Int: 2, Int64: 2L, Double: 2.2, String: "2.2.2"},
            Job{Bool: false, Int: 3, Int64: 3L, Double: 3.3, String: "3.3.3"},
        },
        Job[]{
            Job{Bool: true, Int: 4, Int64: 4L, Double: 4.4, String: "4.4.4"},
            Job{Bool: false, Int: 5, Int64: 5L, Double: 5.5, String: "5.5.5"},
        },
        Job[]{
            Job{Bool: true, Int: 6, Int64: 6L, Double: 6.6, String: "6.6.6"},
            Job{Bool: false, Int: 7, Int64: 7L, Double: 7.7, String: "7.7.7"},
        }
    };

    // 测试不同长度数组的切片
    test_string_slices(local_string_array_2);
}
fn test_string_slices(Job[,] arr) {
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
	test_slice_class_array();
}