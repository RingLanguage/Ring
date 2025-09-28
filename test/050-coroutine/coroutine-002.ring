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
	var int64 consumerid;
}


fn producer() {
	var int   producer_count = 10;
	var int i = 0;

	fmt::printf("producer start work\n");

	for i = 0; i < producer_count; i++ {
		fmt::printf("producer {}\n", i);
		resume(consumerid);
	}

	fmt::printf("producer end work\n");
}


fn consumer() {
	var int   consumer_count;
	fmt::printf("consumer start work\n");
    for ;; {
		yield();
		fmt::printf("consumer {}\n", consumer_count);
		consumer_count++;
	}
}

// TE: 测试协程 yield resume
// TE: 两个协程，一个生产者，一个消费者
// TE: 生产者是个循环，每次生产一个数据，就resume一下消费者
// TE: 消费者是个循环，每次消费一个数据，就yield一下
// TE: 生产者生产一定数量，就返回 main 函数，整个进程结束
// TE: 数据不是通过管道传送的，每个协程自己计数
fn main() {

	consumerid = launch consumer();
	resume(consumerid);

	producer();

	fmt::printf("main end work\n");
}