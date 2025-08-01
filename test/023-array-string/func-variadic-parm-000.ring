package main

import {
    fmt;
    debug;
}

global {
}

// TestDetail:测试可变参数
// TestDetail:可变参数为 string... 类型
fn main() {
    fmt::println_string("test case 1--------");
    func_variadic_parameter("#1");

    fmt::println_string("\ntest case 2--------");
    func_variadic_parameter("#1", "#2");

    fmt::println_string("\ntest case 3--------");
    func_variadic_parameter("#1", "#2", "#3");

    fmt::println_string("\ntest case 4--------");
    func_variadic_parameter("#1", "#2", "#3", "#4");
}


fn func_variadic_parameter(var string... array_value) {
    fmt::printf("len(array_value)      = {}\n", len(array_value));
    fmt::printf("cap(array_value) = {}\n", cap(array_value));

    var string string_value;
    for string_value = range array_value {
        fmt::println(string_value);
    }
}
