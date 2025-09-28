//
// File:    std-debug-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-24
//

package main

import {
	os;
	debug;
	fmt;
}


fn job_action1() {
	fmt::println("        job_action1::1");
	yield();
	fmt::println("        job_action1::2");
}


// TE: 测试协程简单的 yield resume
fn main() {
	var int64 co_id;

	fmt::println("main start");

	co_id = launch job_action1();
	fmt::printf("main co_id: {}\n", co_id);

	fmt::printf("\n\n");

	
	fmt::printf("main before resume co_id: {}\n", co_id);
	resume(co_id);
	fmt::printf("main after resume co_id: {}\n", co_id);


	fmt::printf("\n\n");


	fmt::printf("main before resume co_id: {}\n", co_id);
	resume(co_id);
	fmt::printf("main after resume co_id: {}\n", co_id);


}