//
// File:    buildin.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2024-07-01
//

package buildin

import {

}

@native
fn len(any array) -> (int);

@native
fn cap(any array) -> (int);


@native
fn push(any array, any value);

@native
fn pop(any array) -> (any);


@native
fn to_string(any value) -> (string);

@native
fn to_int64(any value) -> (int64);


@native
fn resume(int64 co_id);

@native
fn yield();


