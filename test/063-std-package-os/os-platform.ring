
package main

import {
    fmt;
    debug;
    vm;
    os;
}

global {
}

fn main() {
    var string os;
    var string arch;

    os, arch = os::platform();
    fmt::println(os);
    fmt::println(arch);
}
