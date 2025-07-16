
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
    // fmt::println(os);
    // fmt::println(arch);

    debug::assert(os == "darwin" or os == "linux" or os == "windows");
    debug::assert(arch == "x86_64" or arch == "arm64");
}
