//
// File:    os.ring
// Author:  GeneralSandman - generalsandman@163.com
// CDate:   2023-10-08
// UDate:   2023-10-08
//

package os

import {

}

global {

}

// build-in
@native
fn exit(var int int_value);

@native
fn remove(var string name);

@native
fn getenv(var string name) -> (string);

@native
fn setenv(var string name, var string value);

// return OS Name and Architecture
// e.g. "linux", "x86_64"
// e.g. "windows", "x86_64"
// e.g. "darwin", "x86_64"
@native
fn platform() -> (string, string);