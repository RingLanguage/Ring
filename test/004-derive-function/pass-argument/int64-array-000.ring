package main

import {
	fmt;
	debug;
}


// TestDetail: derive函数传递int64数组类型参数
// TestDetail: 分别传递1-8个int64数组类型参数
fn main() {

	pass_1_int64_array(int64[]{0L});
	pass_1_int64_array(int64[]{1L});

	pass_2_int64_array(int64[]{0L}, int64[]{0L});
	pass_2_int64_array(int64[]{1L}, int64[]{1L});

	pass_3_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_3_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L});

	pass_4_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_4_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L});

	pass_5_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_5_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L});

	pass_6_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_6_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L});

	pass_7_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_7_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L});

	pass_8_int64_array(int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L}, int64[]{0L});
	pass_8_int64_array(int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L}, int64[]{1L});
}





fn pass_1_int64_array(int64[] a) {
	fmt::println(a);
}

fn pass_2_int64_array(int64[] a, int64[] b) {
	fmt::println(a, b);
}

fn pass_3_int64_array(int64[] a, int64[] b, int64[] c) {
    fmt::println(a, b, c);
}

fn pass_4_int64_array(int64[] a, int64[] b, int64[] c, int64[] d) {
    fmt::println(a, b, c, d);
}

fn pass_5_int64_array(int64[] a, int64[] b, int64[] c, int64[] d, int64[] e) {
    fmt::println(a, b, c, d, e);
}

fn pass_6_int64_array(int64[] a, int64[] b, int64[] c, int64[] d, int64[] e, int64[] f) {
    fmt::println(a, b, c, d, e, f);
}

fn pass_7_int64_array(int64[] a, int64[] b, int64[] c, int64[] d, int64[] e, int64[] f, int64[] g) {
    fmt::println(a, b, c, d, e, f, g);
}

fn pass_8_int64_array(int64[] a, int64[] b, int64[] c, int64[] d, int64[] e, int64[] f, int64[] g, int64[] h) {
    fmt::println(a, b, c, d, e, f, g, h);
}

