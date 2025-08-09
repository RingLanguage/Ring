package encoding

import {

}

@native
fn json_encode(any any_value) -> (string);

@native
fn json_encode_indent(any any_value) -> (string);

// return parse result and error string
@native
fn json_decode(any any_value, string json_string) -> (any, string);
