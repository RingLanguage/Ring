package encoding

import {

}

@native
fn json_encode(var any any_value) -> (string);

@native
fn json_encode_indent(var any any_value) -> (string);

@native
// return parse result and error string
fn json_decode(var any any_value, var string json_string) -> (any, string);
