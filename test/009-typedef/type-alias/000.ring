
package main

import {
	fmt;
	debug;
}

global {
}


typedef TaskIsBilling  = bool;
typedef TaskCreateTime = int;
typedef TaskID         = int64;
typedef TaskScore      = double;
typedef TaskStatus     = string;


fn main() {
	var TaskIsBilling  task_is_billing;
	var TaskCreateTime task_created_time;
	var TaskID         task_id;
	var TaskScore      task_score;
	var TaskStatus     task_status;

	task_is_billing 	= true;
	task_created_time 	= 1234567890;
	task_id 			= 1234567890123456789L;
	task_score 			= 3.14;
	task_status 		= "pending";


	fmt::println("-----------------\n");
	fmt::println(task_is_billing);
	fmt::println(task_created_time);
	fmt::println(task_id);
	fmt::println(task_score);
	fmt::println(task_status);


	fmt::println("-----------------\n");
	formatTaskIsBilling(task_is_billing);
	formatTaskCreateTime(task_created_time);
	formatTaskID(task_id);
	formatTaskScore(task_score);
	formatTaskStatus(task_status);


	fmt::println("-----------------\n");
	formatTaskIsBilling(true);
	formatTaskCreateTime(1234567890);
	formatTaskID(1234567890123456789L);
	formatTaskScore(3.14);
	formatTaskStatus("completed");

}

fn formatTaskIsBilling(TaskIsBilling is_billing) {
	if is_billing {
		fmt::println("Task is billing.");
	} else {
		fmt::println("Task is not billing.");
	}
}

fn formatTaskCreateTime(TaskCreateTime create_time) {
	fmt::println("Task created at: ", create_time);
}

fn formatTaskID(TaskID id) {
	fmt::println("Task ID: ", id);
}

fn formatTaskScore(TaskScore score) {
	fmt::println("Task score: ", score);
}

fn formatTaskStatus(TaskStatus status) {
	if status == "pending" {
		fmt::println("Task is pending.");
	} else if status == "completed" {
		fmt::println("Task is completed.");
	} else if status == "failed" {
		fmt::println("Task has failed.");
	} else {
		fmt::println("Unknown task status.");
	}
}
