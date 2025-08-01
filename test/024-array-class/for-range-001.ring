package main

import {
    fmt;
    debug;
}

typedef Job1 = class {
    var bool    Bool;
    var int     Int;
    var double  Double;
    var string  String;
}

global {
  var Job1[,,] global_job1_array_0;
}

// TestDetail: 测试对三维数组进行 for-range 遍历
fn main() {
	global_job1_array_0 = Job1[,,]{
		Job1[,]{
			Job1[]{
				Job1{Bool: true,  Int: 0, Double: 0.0, String: "0.0"},
				Job1{Bool: false, Int: 1, Double: 1.1, String: "1.1"},
				Job1{Bool: true,  Int: 2, Double: 2.2, String: "2.2"}
			},
			Job1[]{
				Job1{Bool: false, Int: 3, Double: 3.3, String: "3.3"},
				Job1{Bool: true,  Int: 4, Double: 4.4, String: "4.4"},
				Job1{Bool: false, Int: 5, Double: 5.5, String: "5.5"}
			},
			Job1[]{
				Job1{Bool: true,  Int: 6, Double: 6.6, String: "6.6"},
				Job1{Bool: false, Int: 7, Double: 7.7, String: "7.7"},
				Job1{Bool: true,  Int: 8, Double: 8.8, String: "8.8"}
			}
		},
		Job1[,]{
			Job1[]{
				Job1{Bool: false, Int: 9, Double: 9.9, String: "9.9"},
				Job1{Bool: true,  Int: 10, Double: 10.10, String: "10.10"},
				Job1{Bool: false, Int: 11, Double: 11.11, String: "11.11"}
			},
			Job1[]{
				Job1{Bool: true,  Int: 12, Double: 12.12, String: "12.12"},
				Job1{Bool: false, Int: 13, Double: 13.13, String: "13.13"},
				Job1{Bool: true,  Int: 14, Double: 14.14, String: "14.14"}
			},
			Job1[]{
				Job1{Bool: false, Int: 15, Double: 15.15, String: "15.15"},
				Job1{Bool: true,  Int: 16, Double: 16.16, String: "16.16"},
				Job1{Bool: false, Int: 17, Double: 17.17, String: "17.17"}
			}
		},
		Job1[,]{
			Job1[]{
				Job1{Bool: true,  Int: 18, Double: 18.18, String: "18.18"},
				Job1{Bool: false, Int: 19, Double: 19.19, String: "19.19"},
				Job1{Bool: true,  Int: 20, Double: 20.20, String: "20.20"}
			},
			Job1[]{
				Job1{Bool: false, Int: 21, Double: 21.21, String: "21.21"},
				Job1{Bool: true,  Int: 22, Double: 22.22, String: "22.22"},
				Job1{Bool: false, Int: 23, Double: 23.23, String: "23.23"}
			},
			Job1[]{
				Job1{Bool: true,  Int: 24, Double: 24.24, String: "24.24"},
				Job1{Bool: false, Int: 25, Double: 25.25, String: "25.25"},
				Job1{Bool: true,  Int: 26, Double: 26.26, String: "26.26"}
			}
		}
	};

	debug::assert(len(global_job1_array_0) == 3);
	debug::assert(len(global_job1_array_0[0]) == 3);
	debug::assert(len(global_job1_array_0[1]) == 3);
	debug::assert(len(global_job1_array_0[2]) == 3);


	var int i;
    var int j;
    var int k;
    var int num;
	var bool bool_value = true;

	var Job1[,] tmp_2;
	var Job1[]  tmp_1;
	var Job1    tmp_0;

	for tmp_2 = range global_job1_array_0 {
		for tmp_1 = range tmp_2 {
		    for tmp_0 = range tmp_1 {
				if tmp_0.Bool != bool_value {
					fmt::println_string("test failed");
				}
				if tmp_0.Int != num {
					fmt::println_string("test failed");
				} 

				num++;
				bool_value = not bool_value;
			}
		}
	}

    debug::assert(num == 27);
}