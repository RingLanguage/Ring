//
// File:    std-debug-000.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-24
//

package main

import {
	debug;
	fmt;
}

global {

	var int64 consumer_1_id;
	var int64 consumer_2_id;
}


fn producer() {
	var int   producer_count = 10;
	var int i = 0;

	fmt::printf("producer start work\n");

	for i = 0; i < producer_count; i++ {
		fmt::printf("producer   {}\n", i);
		resume(consumer_1_id);
	}

	fmt::printf("producer end work\n");
}


fn consumer_1() {
	var int   consumer_1_count;
	fmt::printf("consumer_1 start work\n");
    for ;; {
		yield();
		fmt::printf("consumer_1 {}\n", consumer_1_count);
		consumer_1_count++;
		resume(consumer_2_id);
	}
}

fn consumer_2() {
	var int   consumer_2_count;
	fmt::printf("consumer_2 start work\n");
    for ;; {
		yield();
		fmt::printf("consumer_2 {}\n", consumer_2_count);
		consumer_2_count++;
	}
}

// TE: 测试协程 yield resume
// TE: 三个协程：producer, consumer_1, consumer_2
// TE: producer 生产 10 个数据，每个数据 consumer_1 消费一次，consumer_2 消费一次
fn main() {

	consumer_1_id = launch consumer_1();
	resume(consumer_1_id);

	consumer_2_id = launch consumer_2();
	resume(consumer_2_id);

	producer();

	fmt::printf("main end work\n");
}