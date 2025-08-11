package main

import {
    fmt;
    debug;
}

typedef FuncType = fn() -> (int);


fn test_slice_bool_array() {
    fmt::printf("test_slice_bool_array ------------\n");

    var FuncType[] local_bool_array_2 = FuncType[]{
        fn() -> (int) {
            fmt::printf("Function 0 called\n");
            return 0;
        },
        fn() -> (int) {
            fmt::printf("Function 1 called\n");
            return 1;
        },
        fn() -> (int) {
            fmt::printf("Function 2 called\n");
            return 2;
        },
        fn() -> (int) {
            fmt::printf("Function 3 called\n");
            return 3;
        },
        fn() -> (int) {
            fmt::printf("Function 4 called\n");
            return 4;  
        },
        fn() -> (int) {
            fmt::printf("Function 5 called\n");
            return 5;
        }
    };

    // 测试不同长度数组的切片
    test_bool_slices(local_bool_array_2);
}
fn test_bool_slices(FuncType[] arr) {
    var int len = len(arr);
	var int start = 0;
	var int end = 0;

    // 测试 start 和 end 都存在的情况
    for start = 0; start < len; start++ {
        for end = start; end < len+1; end++ {
            fmt::printf("arr[{}:{}] = {}\n", start, end, len(arr[start:end]));
        }
    }

    // 测试 start 不存在的情况
    for end = 0; end < len+1; end++ {
        fmt::printf("arr[:{}] = {}\n", end, len(arr[:end]));
    }

    // 测试 end 不存在的情况
    for start = 0; start < len+1; start++ {
        fmt::printf("arr[{}:] = {}\n", start, len(arr[start:]));
    }

    // 测试 start 和 end 都不存在的情况
    fmt::printf("arr[:] = {}\n", len(arr[:]));

    var FuncType func_item;
    var int result;
    for _, func_item = range arr {
        result = func_item();
        fmt::printf("Function returned: {}\n", result);
    }
}


fn main() {
	test_slice_bool_array();
}