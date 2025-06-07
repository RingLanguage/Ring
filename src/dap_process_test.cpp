#include "dap.hpp"
#include "dap_process.hpp"
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unistd.h>


// event
int test_send_event_1() {

    printf("test_send_event_1------------\n");


    DapMessageSender      sender(STDOUT_FILENO);


    dap::InitializedEvent initialized_event = dap::InitializedEvent{
        .seq = 0,
    };
    sender.send(initialized_event);


    dap::StoppedEvent stopped_event = dap::StoppedEvent{
        .seq = 1,
    };
    sender.send(stopped_event);


    dap::OutputEvent output_event = dap::OutputEvent{
        .seq = 2,
    };
    sender.send(output_event);


    return 0;
}

// response
int test_send_response_1() {

    printf("test_send_response_1------------\n");

    DapMessageSender sender(STDOUT_FILENO);


    {
        // initialize_response
        dap::InitializeResponse initialize_response = dap::InitializeResponse{
            {
                .seq         = 1,
                .request_seq = 1,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_Initialize,
                .success     = true,
                .message     = "",
            },
            .body = dap::InitializeResponseBody{
                .supportsConfigurationDoneRequest = true,
                .supportsFunctionBreakpoints      = true,
                .supportsConditionalBreakpoints   = true,
                .exceptionBreakpointFilters       = std::vector<dap::ExceptionBreakpointsFilter>{},
                .supportsSetVariable              = true,
                .supportsStepBack                 = true,
            },
        };
        sender.send(initialize_response);
    }
    {
        // launch_response
        dap::LaunchResponse launch_response = dap::LaunchResponse{
            {
                .seq         = 2,
                .request_seq = 2,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_Launch,
                .success     = true,
                .message     = "",
            },
            .body = dap::LaunchResponseBody{
                .error = dap::LaunchErrorBody{
                    .id        = 1,
                    .format    = "Launch failed",
                    .variables = {{"reason", "Invalid program path"}},
                },
            },
        };
        sender.send(launch_response);
    }
    {
        // threads_response
        dap::ThreadsResponse threads_response = dap::ThreadsResponse{
            {
                .seq         = 1,
                .request_seq = 1,
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
        sender.send(threads_response);
    }
    {
        dap::StackTraceResponse stack_trace_response = dap::StackTraceResponse{
            {
                .seq         = 2,
                .request_seq = 2,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_StackTrace,
                .success     = true,
                .message     = "",
            },
            .body = dap::StackTraceResponseBody{
                .stackFrames = std::vector<dap::StackFrame>{
                    {
                        .id        = 0,
                        .name      = "main",
                        .line      = 10,
                        .column    = 5,
                        .endLine   = 10,
                        .endColumn = 5,
                    },
                },
                .totalFrames = 1,
            }};
        sender.send(stack_trace_response);
    }
    {
        dap::ContinueResponse continue_response = dap::ContinueResponse{
            {
                .seq         = 3,
                .request_seq = 3,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_Continue,
                .success     = true,
                .message     = "",
            },
            .body = dap::ContinueResponseBody{
                .allThreadsContinued = true,
            },
        };
        sender.send(continue_response);
    }
    {
        dap::NextResponse step_response = dap::NextResponse{
            {
                .seq         = 3,
                .request_seq = 3,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_Next,
                .success     = true,
                .message     = "",
            },
            .body = dap::StepResponseBody{},
        };
        sender.send(step_response);
    }
    {
        dap::StepInResponse stepin_response = dap::StepInResponse{
            {
                .seq         = 3,
                .request_seq = 3,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_StepIn,
                .success     = true,
                .message     = "",
            },
            .body = dap::StepResponseBody{},
        };
        sender.send(stepin_response);
    }
    {
        dap::StepOutResponse stepout_response = dap::StepOutResponse{
            {
                .seq         = 3,
                .request_seq = 3,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_StepOut,
                .success     = true,
                .message     = "",
            },
            .body = dap::StepResponseBody{},
        };
        sender.send(stepout_response);
    }
    {
        dap::SetBreakpointsResponse set_breakpoints_response = dap::SetBreakpointsResponse{
            {
                .seq         = 3,
                .request_seq = 3,
                .type        = dap::MessageType_Response,
                .command     = dap::Command_SetBreakpoints,
                .success     = true,
                .message     = "",
            },
            .body = dap::SetBreakpointsResponseBody{
                .breakpoints = std::vector<dap::BreakpointResponseInfo>{
                    {.id       = 1,
                     .line     = 10,
                     .verified = true,
                     .message  = "",
                     .source   = dap::Source{
                           .name            = "test.ring",
                           .path            = "/home/user/test.ring",
                           .sourceReference = 10,
                     }}},
            },
        };
        sender.send(set_breakpoints_response);
    }


    return 0;
}

void testMessageHandler(const std::string& body) {
    if (body.empty()) {
        std::cerr << "-Received empty message" << std::endl;
        return;
    }

    dap::DAPMessage dap_message;

    auto            err = json_decode(body, &dap_message);
    if (err != nullptr) {
        std::cerr << "Error (" << static_cast<int>(err->type) << "): " << err->message << "\n";
        return;
    }

    std::cout << "+Received message: command=" << dap_message.command << std::endl;
    if (dap_message.command == "threads") {
        dap::ThreadsRequest threads_request;
        err = json_decode(body, &threads_request);
        std::cout << "+ThreadsRequest: \n"
                  << "seq=" << threads_request.seq
                  << ", type=" << threads_request.type
                  << ", command=" << threads_request.command
                  << std::endl;
    } else if (dap_message.command == "stackTrace") {
        dap::StackTraceRequest stack_trace_request;
        err = json_decode(body, &stack_trace_request);
        std::cout << "+StackTraceRequest: \n"
                  << "seq=" << stack_trace_request.seq
                  << ", type=" << stack_trace_request.type
                  << ", command=" << stack_trace_request.command
                  << std::endl;

        std::cout << "\t arguments:"
                  << "threadId:" << stack_trace_request.arguments.threadId
                  << std::endl;

    } else if (dap_message.command == "continue") {

        dap::ContinueRequest continue_request;
        err = json_decode(body, &continue_request);
        std::cout << "+ContinueRequest: \n"
                  << "seq=" << continue_request.seq
                  << ", type=" << continue_request.type
                  << ", command=" << continue_request.command
                  << std::endl;

        std::cout << "\t arguments:"
                  << "threadId:" << continue_request.arguments.threadId
                  << ", singleThread:" << (continue_request.arguments.singleThread ? *continue_request.arguments.singleThread : false)
                  << std::endl;

    } else {
        std::cerr << "Unknown command: " << dap_message.command << "\n";
        return;
    }

    if (err != nullptr) {
        std::cerr << "Error (" << static_cast<int>(err->type) << "): " << err->message << "\n";
        return;
    }
}

// 循环处理消息
int process_dap_handler_test_1() {

    DapMessageProcessor processor(STDIN_FILENO, testMessageHandler);
    printf("Starting message processor...\n");
    processor.run();

    return 0;
}

// 一次只处理一个消息
int process_dap_handler_test_2() {

    DapMessageProcessor processor(STDIN_FILENO, testMessageHandler);
    printf("Starting message processor...\n");

    while (true) {
        std::string message = processor.get_a_message();
        printf("-Received message: %s\n", message.c_str());
        if (message == "q") {
            printf("Exiting message processor...\n");
            break;
        }
        testMessageHandler(message);
    }


    return 0;
}

/*

缺失seq 字段
{"seq1":1, "type":"request", "command":"initialize"}

正常
{"seq":1, "type":"request", "command":"threads", "arguments":{}}

{"seq":1, "type":"request", "command":"stackTrace", "arguments":{}}
{"seq":1, "type":"request", "command":"stackTrace", "arguments":{"threadId":1,"startFrame":0,"levels":20}}

continue
{"seq":1, "type":"request", "command":"continue", "arguments":{"threadId":1,"singleThread":false}}

next
{"seq":1, "type":"request", "command":"next", "arguments":{"threadId":1,"singleThread":false, "granularity":""}}

setBreakpoints
{"type":"request","seq":14,"command":"setBreakpoints","arguments":{"source":{"name":"","path":"/Users/lizhenhu/Desktop/vscode-ring-debugger-extension/sampleWorkspace/function-000.ring","sourceReference":0},"lines":[38],"breakpoints":[{"line":38}],"sourceModified":false}}
*/


/*

int dap_process_test_main() {
    test_send_event_1();
    test_send_response_1();

    // process_dap_handler_test_1();
    // process_dap_handler_test_2();

    return 0;
}

*/