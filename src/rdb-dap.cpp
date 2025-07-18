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


    dap_rdb_message_process_loop(frame, event, arg);

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
        // check hit breakpoints
        int hit_breakpoint_num = -1;

        for (int i = 0; i < break_points.size(); i++) {
            if (break_points[i].line_number == frame->source_line_number) {
                hit_breakpoint_num = i;
                break;
            }
        }

        if (hit_breakpoint_num != -1) {
            // 命中断点
            // TODO: 这时候 reason 应该为 Breakpoint
        } else {
            return 0; // 没有命中断点
        }
    }

    // 发送 stop 事件，reason 先写死 step 即可
    dap::StoppedEvent stopped_event;
    stopped_event.seq                    = dap_seq++;
    stopped_event.body.reason            = dap::StoppedEventReason_Step;
    stopped_event.body.threadId          = 1; // TODO: 获取当前线程ID
    stopped_event.body.allThreadsStopped = true;

    DapMessageSender sender(STDERR_FILENO);
    sender.send(stopped_event);

    dap_rdb_message_process_loop(frame, event, arg);


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


int dap_dispath_exit(RVM_Frame* frame, const char* event, const char* arg) {

    dap::ExitedEvent exited_event;
    exited_event.seq           = dap_seq++;
    exited_event.body.exitCode = frame->exit_code;

    DapMessageSender sender(STDERR_FILENO);
    sender.send(exited_event);

    return 0;
}

std::variant<int, dap::LaunchRequest>
dap_rdb_message_process_loop_norun(RVM_DebugConfig* debug_config) {

    std::vector<RVM_BreakPoint>& break_points = debug_config->break_points;

    DapMessageProcessor          dap_processor(STDIN_FILENO, nullptr);
    DapMessageSender             dap_sender(STDERR_FILENO);


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
        if (dap_message.command == dap::Command_Launch) {
            break_read_input = true;

            dap::LaunchRequest request;
            auto               err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode LaunchRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }

            dap::LaunchResponse response = dap::LaunchResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_Launch,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::LaunchResponseBody{
                    // .error = nullptr,
                },
            };

            dap_sender.send(response);
            return request;

        } else if (dap_message.command == dap::Command_SetBreakpoints) {
            break_read_input = false;

            dap::SetBreakpointsRequest request;
            auto                       err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode SetBreakpointsRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }

            dap::SetBreakpointsResponse response = dap::SetBreakpointsResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_SetBreakpoints,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::SetBreakpointsResponseBody{
                    .breakpoints = std::vector<dap::BreakpointResponseInfo>{},
                },
            };

            break_points.clear();
            int break_points_count = 0;
            // TODO: 这里有很多东西是写死的
            for (dap::Breakpoint& bp : request.arguments.breakpoints) {

                RVM_BreakPoint breakpoint = RVM_BreakPoint{
                    .package     = nullptr,
                    .file_name   = nullptr,
                    .func_name   = nullptr,
                    .line_number = (unsigned int)(bp.line),
                };
                break_points.push_back(breakpoint);

                response.body.breakpoints.push_back(dap::BreakpointResponseInfo{
                    .id       = break_points_count++,
                    .line     = bp.line,
                    .verified = true,
                    .message  = "",
                    .source   = request.arguments.source,
                });
            }

            dap_sender.send(response);

        } else if (dap_message.command == dap::Command_Threads) {
        } else if (dap_message.command == dap::Command_StackTrace) {
        } else if (dap_message.command == dap::Command_Scopes) {
        } else if (dap_message.command == dap::Command_Variables) {
        } else if (dap_message.command == dap::Command_Continue) {
        } else if (dap_message.command == dap::Command_Next) {
        } else if (dap_message.command == dap::Command_StepIn) {
        } else if (dap_message.command == dap::Command_StepOut) {
        }


        if (break_read_input) {
            break;
        }
    }


    return 0;
}


// 1. 从 stdin 接受dap 消息
// 2. 解析消息
// 3. 根据消息类型，调用对应的处理函数
// 4. 如果是 continue 命令，退出 循环，不再处理dap消息
// 5. 如果是 其他命令，循环处理dap 消息
int dap_rdb_message_process_loop(RVM_Frame* frame, const char* event, const char* arg) {

    RVM_DebugConfig*             debug_config = frame->rvm->debug_config;
    std::vector<RVM_BreakPoint>& break_points = frame->rvm->debug_config->break_points;

    DapMessageProcessor          dap_processor(STDIN_FILENO, nullptr);
    DapMessageSender             dap_sender(STDERR_FILENO);

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
            break_read_input                      = false;

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
            break_read_input = false;

            dap::StackTraceRequest request;
            auto                   err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode StackTraceRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }


            unsigned int stack_level   = get_rvm_call_stack_level(frame->rvm);

            int          start_frame   = request.arguments.startFrame;
            int          levels_to_get = request.arguments.levels; // 0表示获取全部
            start_frame                = std::max(0, start_frame);
            levels_to_get              = std::max(0, levels_to_get);
            int endFrame               = levels_to_get > 0 ? start_frame + levels_to_get : stack_level;
            endFrame                   = std::min((unsigned int)(endFrame), stack_level);

            std::vector<dap::StackFrame> stack_frames;
            for (int level = start_frame; level < endFrame; level++) {
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

        } else if (dap_message.command == dap::Command_Scopes) {
            // 先发送 response
            // 继续处理消息
            break_read_input = false;

            dap::ScopesRequest request;
            auto               err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode ScopesRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }

            dap::ScopesResponse response = dap::ScopesResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_Scopes,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::ScopesResponseseBody{
                    .scopes = std::vector<dap::Scope>{
                        {
                            .name               = "Local",
                            .variablesReference = 1001,
                            .expensive          = true,
                        },
                        {
                            .name               = "Global",
                            .variablesReference = 1002,
                            .expensive          = true,
                        },
                    },
                },
            };
            dap_sender.send(response);

        } else if (dap_message.command == dap::Command_Variables) {
            // 先发送 response
            // 继续处理消息
            break_read_input = false;

            dap::VariablesRequest request;
            auto                  err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode VariablesRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }

            dap::VariablesResponse response = dap::VariablesResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_Variables,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::VariablesResponseBody{
                    .variables = std::vector<dap::Variable>{},
                },
            };

            if (request.arguments.variablesReference == 1001) {
                // local
                for (std::pair<std::string, RVM_Value*>& local : frame->locals) {
                    std::string type  = format_rvm_type(frame->rvm, local.second);
                    std::string value = format_rvm_value(local.second);
                    response.body.variables.push_back(dap::Variable{
                        .name  = local.first,
                        .type  = type,
                        .value = type + " " + value, // TODO: 对于基础类型，是否没必要展示 type
                        // TODO: 如果数据太多，需要考虑分页
                    });
                }
            } else if (request.arguments.variablesReference == 1002) {
                // global
                for (std::pair<std::string, RVM_Value*>& local : frame->globals) {
                    std::string type  = format_rvm_type(frame->rvm, local.second);
                    std::string value = format_rvm_value(local.second);
                    response.body.variables.push_back(dap::Variable{
                        .name  = local.first,
                        .type  = type,
                        .value = type + " " + value, // TODO: 对于基础类型，是否没必要展示 type
                        // TODO: 如果数据太多，需要考虑分页
                    });
                }
            }
            dap_sender.send(response);
        } else if (dap_message.command == dap::Command_SetBreakpoints) {
            // 先发送 response
            // 继续处理消息
            break_read_input = false;

            dap::SetBreakpointsRequest request;
            auto                       err = json_decode(message_body, &request);
            if (err != nullptr) {
                // 错误处理
                printf("json_decode SetBreakpointsRequest error:%s", err->message.c_str());
                fflush(stdout);
                continue;
            }

            dap::SetBreakpointsResponse response = dap::SetBreakpointsResponse{
                {
                    .seq         = dap_seq++,
                    .request_seq = dap_message.seq,
                    .type        = dap::MessageType_Response,
                    .command     = dap::Command_SetBreakpoints,
                    .success     = true,
                    .message     = "",
                },
                .body = dap::SetBreakpointsResponseBody{
                    .breakpoints = std::vector<dap::BreakpointResponseInfo>{},
                },
            };

            break_points.clear();
            int break_points_count = 0;
            // TODO: 这里有很多东西是写死的
            for (dap::Breakpoint& bp : request.arguments.breakpoints) {

                RVM_BreakPoint breakpoint = RVM_BreakPoint{
                    .package     = nullptr,
                    .file_name   = nullptr,
                    .func_name   = nullptr,
                    .line_number = (unsigned int)(bp.line),
                };
                break_points.push_back(breakpoint);

                response.body.breakpoints.push_back(dap::BreakpointResponseInfo{
                    .id       = break_points_count++,
                    .line     = bp.line,
                    .verified = true,
                    .message  = "",
                    .source   = request.arguments.source,
                });
            }

            dap_sender.send(response);

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