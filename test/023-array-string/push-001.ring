package main

import {
    fmt;
    debug;
}

global {
  var string[] global_string_array_0;
}

fn main() {
    var string string_value;

    fmt::println_string("test cast 0 length&capacity------------------------");
    global_string_array_0 = new string[0];
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 0);
    debug::assert(cap(global_string_array_0) == 0);


    fmt::println_string("\ntest cast 1 length&capacity------------------------");
    push(global_string_array_0, "hello world");
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 1);
    debug::assert(cap(global_string_array_0) == 8);

    fmt::println_string("\ntest cast 2 length&capacity------------------------");
    push(global_string_array_0, "hello world");
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 2);
    debug::assert(cap(global_string_array_0) == 8);

    fmt::println_string("\ntest cast 3 length&capacity------------------------");
    push(global_string_array_0, "hello world");
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 3);
    debug::assert(cap(global_string_array_0) == 8);

    fmt::println_string("\ntest cast 4 length&capacity------------------------");
    push(global_string_array_0, "hello world");
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 4);
    debug::assert(cap(global_string_array_0) == 8);

    fmt::println_string("\ntest cast 5 length&capacity------------------------");
    push(global_string_array_0, "hello world");
    fmt::println_int(len(global_string_array_0));
    fmt::println_int(cap(global_string_array_0));
    debug::assert(len(global_string_array_0) == 5);
    debug::assert(cap(global_string_array_0) == 8);




    fmt::println_string("\ntest cast 6 value------------------------");
    debug::assert(global_string_array_0[0]  == "hello world");
    debug::assert(global_string_array_0[1]  == "hello world");
    debug::assert(global_string_array_0[2]  == "hello world");
    debug::assert(global_string_array_0[3]  == "hello world");
    debug::assert(global_string_array_0[4]  == "hello world");


    fmt::println_string("\ntest cast 7 value------------------------");
    for string_value = range global_string_array_0 {
        debug::assert(string_value  == "hello world");
    }
}