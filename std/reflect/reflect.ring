//
// File:    reflect.ring
// Author:  GeneralSandman - generalsandman@163.com
// CDate:   2023-10-07
// UDate:   2023-10-07
//

package reflect

import {

}

@native
fn typeof(any any_value) -> (string);

@native
fn get_obj_fields(any class_obj) -> (string[]);

@native
fn get_obj_field_type(any class_obj, string field_name) -> (string);

@native
fn get_obj_field_value(any class_obj, string field_name) -> (any);

@native
fn set_obj_field_value(any class_obj, string field_name, any value);
