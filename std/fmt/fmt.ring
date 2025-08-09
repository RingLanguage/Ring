//
// File:    fmt.ring
// Author:  GeneralSandman - generalsandman@163.com
// CDate:   2023-04-14
// UDate:   2023-04-19
//

package fmt

import {

}

@native
fn println_bool(bool bool_value);

@native
fn println_int(int int_value);

@native
fn println_int64(int64 int64_value);

@native
fn println_double(double double_value);

@native
fn println_string(string string_value);

@native
fn println_pointer(any any_value);

@native
fn println(any... any_value);

@native
fn printf(string format, any... any_value);

@native
fn sprintf(string format, any... any_value) -> (string);