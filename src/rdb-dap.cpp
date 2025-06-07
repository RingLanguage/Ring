#include "dap.hpp"
#include "dap_process.hpp"
#include "json.hpp"
#include "ring.hpp"
#include <algorithm>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>


static unsigned int trace_count = 0;

/*
 * debug_trace_dispatch_dap
 *
 * 通过 dap 协议来实现调试器的功能
 */
int dap_debug_trace_dispatch(RVM_Frame* frame, const char* event, const char* arg) {

    debug_rdb_with_darkgreen("it is dap process\n");

    RVM_DebugConfig* debug_config = frame->rvm->debug_config;


    debug_rdb_with_darkgreen("---debug_trace_dispatch[%u]---\n", trace_count);
    debug_rdb_with_darkgreen("|[@]event:            %s\n", event);
    debug_rdb_with_darkgreen("|[@]next_opcode:      %d\n", frame->next_opcode);
    debug_rdb_with_darkgreen("|[@]source_line_num:  %u\n", frame->source_line_number);
    debug_rdb_with_darkgreen("---debug_trace_dispatch---\n");
    debug_rdb_with_darkgreen("\n\n");


    if (str_eq(event, TRACE_EVENT_SAE)) {
        if (ISSET_TRACE_EVENT_SAE(debug_config))
            dap_dispath_sae(frame, event, arg);
    } else if (str_eq(event, TRACE_EVENT_OPCODE)) {
        if (ISSET_TRACE_EVENT_OPCODE(debug_config))
            dap_dispath_opcode(frame, event, arg);
    } else if (str_eq(event, TRACE_EVENT_LINE)) {
        if (ISSET_TRACE_EVENT_LINE(debug_config))
            dap_dispath_line(frame, event, arg);
    } else if (str_eq(event, TRACE_EVENT_CALL)) {
        if (ISSET_TRACE_EVENT_CALL(debug_config))
            dap_dispath_call(frame, event, arg);
    } else if (str_eq(event, TRACE_EVENT_RETURN)) {
        if (ISSET_TRACE_EVENT_RETURN(debug_config))
            dap_dispath_return(frame, event, arg);
    } else if (str_eq(event, TRACE_EVENT_EXIT)) {
        if (ISSET_TRACE_EVENT_EXIT(debug_config))
            dap_dispath_exit(frame, event, arg);
    }


    trace_count++;

    return 0;
}


static int dap_seq = 0;

// 1. 发送 stopped 事件
// 2. 等待用户输入命令
int dap_dispath_sae(RVM_Frame* frame, const char* event, const char* arg) {

    debug_rdb_with_darkgreen("dap_dispath_sae\n");

    dap::StoppedEvent stopped_event;
    stopped_event.seq                    = dap_seq++;
    stopped_event.body.reason            = dap::StoppedEventReason_Entry;
    stopped_event.body.threadId          = 1; // TODO: 获取当前线程ID
    stopped_event.body.allThreadsStopped = true;

    DapMessageSender sender(STDERR_FILENO);
    sender.send(stopped_event);


    dap_rdb_cli(frame, event, arg);

    return 0;
}


int dap_dispath_opcode(RVM_Frame* frame, const char* event, const char* arg) {
    return 0;
}


// 也可以暂时不用处理
int dap_dispath_line(RVM_Frame* frame, const char* event, const char* arg) {

    RVM_DebugConfig*             debug_config = frame->rvm->debug_config;
    std::vector<RVM_BreakPoint>& break_points = debug_config->break_points;
    RDB_COMMAND_STEP_TYPE&       step_cmd     = debug_config->step_cmd;
    std::string                  location;

    if (step_cmd != RDB_COMMAND_STEP_UNKNOW) {
        step_cmd = RDB_COMMAND_STEP_UNKNOW;
    } else {
        // 上一个命令是 continue
        // TODO: check 命中断点
        return 0; // 没有命中断点
    }

    // 发送 stop 事件，reason 先写死 step 即可
    dap::StoppedEvent stopped_event;
    stopped_event.seq                    = dap_seq++;
    stopped_event.body.reason            = dap::StoppedEventReason_Step;
    stopped_event.body.threadId          = 1; // TODO: 获取当前线程ID
    stopped_event.body.allThreadsStopped = true;

    DapMessageSender sender(STDERR_FILENO);
    sender.send(stopped_event);

    dap_rdb_cli(frame, event, arg);


    return 0;
}


// 暂时不用处理即可
int dap_dispath_call(RVM_Frame* frame, const char* event, const char* arg) {
    RVM_DebugConfig* debug_config = frame->rvm->debug_config;

    debug_config->call_func_deep_count++; // TODO: 这里多此一举了，应该直接获取栈的数量

    if (debug_config->step_cmd == RDB_COMMAND_STEP_OVER) {
        if (debug_config->step_over_deep_count < debug_config->call_func_deep_count) {
            // 进入一个函数了，不需要 trace_event_line
            // 函数中的断点也会失效
            UNSET_TRACE_EVENT_LINE(debug_config);
        }
    }
    return 0;
}


// TODO:暂时不用处理即可
int dap_dispath_return(RVM_Frame* frame, const char* event, const char* arg) {
    RVM_DebugConfig* debug_config = frame->rvm->debug_config;

    debug_config->call_func_deep_count--; // TODO: 这里多此一举了，应该直接获取栈的数量

    if (debug_config->step_cmd == RDB_COMMAND_STEP_OVER) {
        if (debug_config->step_over_deep_count == debug_config->call_func_deep_count) {
            // 内部的函数返回了，需要继续 trace_event_line
            SET_TRACE_EVENT_LINE(debug_config);
        }
    } else if (debug_config->step_cmd == RDB_COMMAND_STEP_OUT) {
        if (debug_config->step_out_deep_count - 1 == debug_config->call_func_deep_count) {
            SET_TRACE_EVENT_LINE(debug_config);
        }
    }

    return 0;
}


// TODO:
// 1. 发送 exited 事件
// 2. 退出循环
// 3. 进程退出
int dap_dispath_exit(RVM_Frame* frame, const char* event, const char* arg) {
    // printf(LOG_COLOR_GREEN);
    // printf("[@]Process exited, code:%d\n", 0);
    // printf(LOG_COLOR_CLEAR);

    return 0;
}


// 1. 从 stdin 接受dap 消息
// 2. 解析消息
// 3. 根据消息类型，调用对应的处理函数
// 4. 如果是 continue 命令，退出 循环，不再处理dap消息
// 5. 如果是 其他命令，循环处理dap 消息
int dap_rdb_cli(RVM_Frame* frame, const char* event, const char* arg) {

    RVM_DebugConfig*    debug_config = frame->rvm->debug_config;

    DapMessageProcessor dap_processor(STDIN_FILENO, nullptr);
    DapMessageSender    dap_sender(STDERR_FILENO);

    while (true) {

        bool        break_read_input = false;

        std::string message_body     = dap_processor.get_a_message();
        if (message_body.empty()) {
            continue;
        }
        if (message_body == "q") {
            // TODO: 发送 exited 事件
            exit(0);
        }
        dap::DAPMessage dap_message;
        auto            err = json_decode(message_body, &dap_message);
        if (err != nullptr) {
            // 错误处理
            continue;
        }

        debug_rdb_with_darkgreen("dap receive request: %s\n", dap_message.command.c_str());
        // printf("this lizhenhu-debug ring stdout: receive request: %s\n", dap_message.command.c_str());
        // fflush(stdout);

        // TODO: 处理不同的command
        if (dap_message.command == dap::Command_Threads) {
            // TODO: 只返回一个线程即可，当前只有肯定是在 stopped
            // 返回 response即可
            // 继续处理消息

            dap::ThreadsResponse threads_response = dap::ThreadsResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_Threads,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::ThreadsResponseBody{
                    .threads = std::vector<dap::Thread>{
                        {
                            .id          = 1,
                            .name        = "thread-1",
                            .state       = "stopped",
                            .pauseReason = "entry",
                        },
                    },
                },
            };
            dap_sender.send(threads_response);

        } else if (dap_message.command == dap::Command_StackTrace) {
            // TODO: 格式化标准的dap协议，返回
            // 返回 response 即可
            // 继续处理消息

            unsigned int                 stack_level = get_rvm_call_stack_level(frame->rvm);

            std::vector<dap::StackFrame> stack_frames;
            for (int level = 0; level < stack_level; level++) {
                CallInfo call_info = get_rvm_call_stack(frame->rvm, level);

                // 格式化一下
                dap::StackFrame stack_frame = dap::StackFrame{
                    .id     = level,
                    .name   = call_info.func,
                    .source = dap::Source{
                        .name            = "", // FIXME:
                        .path            = call_info.file,
                        .sourceReference = 0, // FIXME:
                    },
                    .line      = int(call_info.line),
                    .column    = 1,                   // FIXME:
                    .endLine   = int(call_info.line), // FIXME:
                    .endColumn = 1,                   // FIXME:
                };
                stack_frames.push_back(stack_frame);
            }

            //
            dap::StackTraceResponse stack_trace_response = dap::StackTraceResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_StackTrace,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::StackTraceResponseBody{
                    .stackFrames = stack_frames,
                    .totalFrames = stack_level,
                }};
            dap_sender.send(stack_trace_response);

        } else if (dap_message.command == dap::Command_Continue) {
            // 先发送 response
            // 直接退出循环即可
            break_read_input                        = true;

            dap::ContinueResponse continue_response = dap::ContinueResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_Continue,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::ContinueResponseBody{
                    .allThreadsContinued = true,
                },
            };
            dap_sender.send(continue_response);
        } else if (dap_message.command == dap::Command_Next) {
            // 先发送 response
            // 直接退出循环即可
            break_read_input                   = true;

            debug_config->step_cmd             = RDB_COMMAND_STEP_OVER;
            debug_config->step_over_deep_count = debug_config->call_func_deep_count;

            dap::NextResponse next_response    = dap::NextResponse{
                   {
                       .seq         = dap_seq++,
                       .request_seq = dap_message.seq,
                       .type        = dap::MessageType_Response,
                       .command     = dap::Command_Next,
                       .success     = true,
                       .message     = "",
                },
                   .body = dap::StepResponseBody{
                       .allThreadsContinued = true,
                },
            };
            dap_sender.send(next_response);
        } else if (dap_message.command == dap::Command_StepIn) {
            // 先发送 response
            // 直接退出循环即可
            break_read_input       = true;

            debug_config->step_cmd = RDB_COMMAND_STEP_INTO;
        } else if (dap_message.command == dap::Command_StepOut) {
            // 先发送 response
            // 直接退出循环即可
            break_read_input                  = true;

            debug_config->step_cmd            = RDB_COMMAND_STEP_OUT;
            debug_config->step_out_deep_count = debug_config->call_func_deep_count;

            UNSET_TRACE_EVENT_LINE(debug_config);
        }


        if (break_read_input) {
            break;
        }
    }


    return 0;
}