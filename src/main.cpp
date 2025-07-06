#include "clipp.h"
#include "dap.hpp"
#include "linenoise.h"
#include "ring.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>


//
int RING_DEBUG_TRACE_FUNC_BACKTRACE     = 0;
int RING_DEBUG_TRACE_COROUTINE_SCHED    = 0;
int RING_DEBUG_TRACE_CLOSURE_FREE_VALUE = 0;

// RING_DEBUG=trace_func_backtrace=1,trace_coroutine_sched=1 ./bin/ring run ./test/050-coroutine/coroutine-002.ring
void ring_parse_env() {
    char* ring_debug_str = getenv("RING_DEBUG");
    if (ring_debug_str == nullptr) {
        return;
    }

    std::vector<std::string> key_values = split(ring_debug_str, ",");
    for (const std::string& str : key_values) {
        std::string              key;
        std::string              value;

        std::vector<std::string> tmp = split(str, "=");
        if (tmp.size() != 2) {
            continue;
        }
        key   = tmp[0];
        value = tmp[1];

        // printf("key: %s, value: %s\n", key.c_str(), value.c_str());

        if (str_eq(key.c_str(), "trace_func_backtrace") && str_eq(value.c_str(), "1")) {
            RING_DEBUG_TRACE_FUNC_BACKTRACE = 1;
        } else if (str_eq(key.c_str(), "trace_coroutine_sched") && str_eq(value.c_str(), "1")) {
            RING_DEBUG_TRACE_COROUTINE_SCHED = 1;
        } else if (str_eq(key.c_str(), "trace_closure_free_value") && str_eq(value.c_str(), "1")) {
            RING_DEBUG_TRACE_CLOSURE_FREE_VALUE = 1;
        }
    }
}

Ring_Command_Arg ring_parse_command(int argc, char** argv) {
    Ring_Command_Arg         args;
    RING_COMMAND_TYPE        cmd = RING_COMMAND_UNKNOW;
    std::string              input_file_name;
    std::string              keyword;
    unsigned int             optimize_level = 0;
    std::string              rdb_interpreter;
    std::vector<std::string> shell_args;


    // option
    auto optimize_level_rule  = (clipp::option("-O", "--Opt") & clipp::value("opt-level", optimize_level));

    auto rdb_interpreter_rule = (clipp::option("", "--interpreter=") & clipp::value("interpreter-protocol", rdb_interpreter));

    auto option_rule          = (optimize_level_rule | rdb_interpreter_rule);

    // run command
    auto run_rule = (clipp::command(RING_CMD_T_RUN).set(cmd, RING_COMMAND_RUN),
                     clipp::value("input-file", input_file_name));

    // build command
    auto build_rule = (clipp::command(RING_CMD_T_BUILD).set(cmd, RING_COMMAND_BUILD),
                       clipp::value("input-file", input_file_name));

    // dump command
    auto dump_rule = ((clipp::command(RING_CMD_T_DUMP).set(cmd, RING_COMMAND_DUMP),
                       clipp::value("input_file_name", input_file_name)));

    // rdb command
    auto rdb_rule = ((clipp::command(RING_CMD_T_RDB).set(cmd, RING_COMMAND_RDB),
                      clipp::value("input_file_name", input_file_name)));

    // shell args
    // e.g.  ./bin/ring run ./test.ring args1 args2
    // shell_args 就是 [args1, args2]
    auto shell_args_rule = clipp::values("shell-args", shell_args);


    auto ring_command_rule =
        ((option_rule, run_rule | build_rule | dump_rule | rdb_rule, shell_args_rule)
         | (clipp::command(RING_CMD_T_MAN).set(cmd, RING_COMMAND_MAN), clipp::value("keyword", keyword))
         | clipp::command(RING_CMD_T_VERSION).set(cmd, RING_COMMAND_VERSION)
         | clipp::command(RING_CMD_T_HELP).set(cmd, RING_COMMAND_HELP));

    clipp::parse(argc, argv, ring_command_rule);


    if (cmd == RING_COMMAND_RUN
        || cmd == RING_COMMAND_DUMP
        || cmd == RING_COMMAND_RDB) {
        shell_args.insert(shell_args.begin(), input_file_name);
    }


    args = Ring_Command_Arg{
        .cmd             = cmd,
        .input_file_name = input_file_name,
        .keyword         = keyword,
        .optimize_level  = optimize_level,
        .rdb_interpreter = rdb_interpreter,
        .shell_args      = shell_args,
    };

    return args;
}


Ring_Command_Arg global_ring_command_arg;

//
Ring_Command_Arg pre_main(int argc, char** argv) {

    // parse env
    ring_parse_env();
    // parse ring command
    global_ring_command_arg = ring_parse_command(argc, argv);

    switch (global_ring_command_arg.cmd) {
    case RING_COMMAND_UNKNOW:
        fprintf(stderr, "Unknow command, type `ring help` find tip.\n");
        exit(ERROR_CODE_COMMAND_ERROR);
        break;
    default:
        break;
    }

    return global_ring_command_arg;
}

//
int main(int argc, char** argv) {

    int exit_code = 0;


#ifdef DEBUG_RVM_INTERACTIVE

    FILE*     fp;
    time_t    t  = time(NULL);
    struct tm tm = *localtime(&t);


    fp           = freopen(DEBUG_RVM_INTERACTIVE_STDOUT_FILE, "a", stdout);
    if (fp == nullptr) {
        ring_error_report("reopen stdout failed\n");
    }


    fprintf(stdout, LOG_COLOR_YELLOW);
    fprintf(stdout, "\n\n"
                    "@Start debug ring virtual machine in interactive mode...\n"
                    "@Date: %d-%02d-%02d %02d:%02d:%02d\n"
                    "@stdout redirect to: %s\n"
                    "@Stdout<<<\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            DEBUG_RVM_INTERACTIVE_STDOUT_FILE);
    fprintf(stdout, LOG_COLOR_CLEAR);

    fflush(stdout);
#endif


    Ring_Command_Arg command_arg;
    command_arg = pre_main(argc, argv);

    if (command_arg.cmd == RING_COMMAND_RUN) {
        exit_code = cmd_handler_run(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_BUILD) {
        exit_code = cmd_handler_build(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_DUMP) {
        exit_code = cmd_handler_dump(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_RDB) {
        exit_code = cmd_handler_rdb(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_VERSION) {
        exit_code = cmd_handler_version(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_MAN) {
        exit_code = cmd_handler_man(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_HELP) {
        exit_code = cmd_handler_help(command_arg);
    }


#ifdef DEBUG_RVM_INTERACTIVE
    fprintf(stdout, LOG_COLOR_YELLOW);
    fprintf(stdout, "@End interactive session.\n"
                    "\n\n");
    fprintf(stdout, LOG_COLOR_CLEAR);
    fclose(stdout);
#endif

    return exit_code;
}


int cmd_handler_run(Ring_Command_Arg command_arg) {
    ExecuterEntry*       executer_entry = nullptr;
    Ring_VirtualMachine* rvm            = nullptr;

    executer_entry                      = ring_compile_main(command_arg.input_file_name, command_arg.shell_args);
    rvm                                 = ring_virtualmachine_create();

    return ring_exec(rvm, executer_entry);
}


int cmd_handler_build(Ring_Command_Arg command_arg) {
    ring_compile_main(command_arg.input_file_name, command_arg.shell_args);
    return 0;
}

int cmd_handler_dump(Ring_Command_Arg command_arg) {
    ExecuterEntry* executer_entry = nullptr;

    executer_entry                = ring_compile_main(command_arg.input_file_name, command_arg.shell_args);
    // Only dump `main` package bytecode detail.
    package_executer_dump(executer_entry->main_package_executer);
    return 0;
}

int cmd_handler_rdb(Ring_Command_Arg command_arg) {
    printf(LOG_COLOR_YELLOW);
    printf("%s\n", RING_VERSION);
    printf("\n");
    printf("Start Ring Debugger...\n");
    printf("\n");
    printf("Input file:%s\n", command_arg.input_file_name.c_str());
    printf("Interpreter:%s\n", command_arg.rdb_interpreter.c_str());
    printf("\n");
    printf(LOG_COLOR_CLEAR);

    int                      exit_code       = 0;
    ExecuterEntry*           executer_entry  = nullptr;
    Ring_VirtualMachine*     rvm             = nullptr;
    RVM_DebugConfig*         debug_config    = nullptr;
    std::string              input_file_name = command_arg.input_file_name;
    std::vector<std::string> shell_args      = command_arg.shell_args;

    debug_config                             = new_debug_config(command_arg);

PROCESS_NEW_SESSION:


    // 处理 rdb 命令
    // 有可能是命令模式，有可能是dap模式
    if (DEBUG_IS_DAP(debug_config)) {
        auto result = dap_rdb_message_process_loop_norun(debug_config);
        if (dap::LaunchRequest* req = std::get_if<dap::LaunchRequest>(&result)) {
            input_file_name             = req->arguments.program;
            shell_args                  = req->arguments.args;

            debug_config->stop_at_entry = req->arguments.stopAtEntry;
        }

    } else {
        RDB_Arg rdb_arg;
        rdb_arg = cli_rdb_message_process_loop_norun(debug_config);
        if (rdb_arg.cmd == RDB_COMMAND_RUN) {
            if (rdb_arg.argument.size()) {
                input_file_name = rdb_arg.argument;
                shell_args      = rdb_arg.shell_args;
            }
        }
    }


    if (input_file_name.empty()) {
        // TODO: error-report
    }


    // 编译
    executer_entry = ring_compile_main(input_file_name, shell_args);
    rvm            = ring_virtualmachine_create();

    // 注册 调试 钩子
    enable_debug_config(debug_config, command_arg);
    register_debugger(rvm, debug_config);

    // 运行
    exit_code = ring_exec(rvm, executer_entry);

    printf(LOG_COLOR_GREEN);
    printf("[@]Process exited, code:%d\n", exit_code);
    printf(LOG_COLOR_CLEAR);

    goto PROCESS_NEW_SESSION;
}

int cmd_handler_version(Ring_Command_Arg command_arg) {
    fprintf(stdout, "%s\n", RING_VERSION);
    return 0;
}

int cmd_handler_man(Ring_Command_Arg command_arg) {
    std::string tmp;
    tmp = get_man_help(command_arg.keyword.c_str());
    fprintf(stdout, "%s\n", tmp.c_str());
    return 0;
}

int cmd_handler_help(Ring_Command_Arg command_arg) {
    std::string tmp;
    tmp = get_help_message();
    fprintf(stdout, "%s\n", tmp.c_str());
    return 0;
}


ExecuterEntry* ring_compile_main(std::string              input_file_name,
                                 std::vector<std::string> shell_args) {

    // 初始化编译阶段的 Memory Pool
    init_front_mem_pool();

    // 初始化语法处理节点相关的struct
    CompilerEntry* compiler_entry = compiler_entry_create();
    // TODO: 目前main package 只能有一个源文件
    // main package 源文件即为 ring run 指定的输入文件
    Package* main_package = package_create_input_file(compiler_entry,
                                                      (char*)PACKAGE_MAIN,
                                                      (char*)input_file_name.c_str());
    // 将 shell_args 注册到 main-package 中
    // 在 main函数中可以通过这种方式获取:
    // func main(var string[] args) { fmt::println(args); }
    main_package->shell_args = shell_args;
    //
    compiler_entry->main_package = main_package;

    // 初始化代码生成阶段相关的struct
    ExecuterEntry* executer_entry = executer_entry_create();


    // Step-0: 预编译官方std包, 并生成vmcode
    compile_std_lib(compiler_entry, executer_entry);

    // Step-1: flex 词法分析，
    // Step-2: bison 语法分析，构建语法树
    // Step-3: 修正语法树
    package_compile(main_package);
    Package_Executer* main_package_executer = package_executer_create(
        executer_entry,
        (char*)PACKAGE_MAIN,
        executer_entry->package_executer_list.size());
    //
    executer_entry->main_package_executer = main_package_executer;

    // Step-4: 生成虚拟机中间代码
    // 这里只是 main package
    ring_generate_vm_code(compiler_entry, executer_entry);

    // Step-5: 链接符号表
    // Complier force destory memory of front-end.
    destory_front_mem_pool();

    return executer_entry;
}


int ring_exec(Ring_VirtualMachine* rvm, ExecuterEntry* executer_entry) {
    int exit_code = 0;

    // Step-6: 加载虚拟机
    ring_virtualmachine_load_executer(rvm, executer_entry);

    // Step-7: 初始化虚拟机
    ring_virtualmachine_init(rvm);

    // Step-8: 运行虚拟机
    try {
        exit_code = ring_execute_vm_code(rvm);
    } catch (const RuntimeException& e) {
        printf("%s\n", e.what());
        printf("exit status 2\n");
        exit(2);
    } catch (...) {
        printf("Caught an unknown exception.\n");
    }

    return exit_code;
}


RVM_DebugConfig* new_debug_config(Ring_Command_Arg args) {
    RVM_DebugConfig* debug_config      = (RVM_DebugConfig*)mem_alloc(NULL_MEM_POOL, sizeof(RVM_DebugConfig));
    debug_config->enable               = true;
    debug_config->trace_dispatch       = nullptr;
    debug_config->enable_trace_event   = 0;
    debug_config->stop_at_entry        = true;
    debug_config->display_globals      = false;
    debug_config->display_locals       = false;
    debug_config->display_call_stack   = false;
    debug_config->step_cmd             = RDB_COMMAND_STEP_UNKNOW;
    debug_config->call_func_deep_count = 0;
    debug_config->step_over_deep_count = 0;
    debug_config->step_out_deep_count  = 0;
    debug_config->break_points         = std::vector<RVM_BreakPoint>{};
    debug_config->rdb_interpreter      = args.rdb_interpreter;

    return debug_config;
}

int enable_debug_config(RVM_DebugConfig* debug_config, Ring_Command_Arg args) {
    SET_TRACE_EVENT_ALL(debug_config);

    if (debug_config->stop_at_entry) {
        SET_TRACE_EVENT_SAE(debug_config);
    } else {
        UNSET_TRACE_EVENT_SAE(debug_config);
    }


    if (DEBUG_IS_DAP(debug_config)) {
        debug_config->trace_dispatch = dap_debug_trace_dispatch;
    } else {
        debug_config->trace_dispatch = cli_debug_trace_dispatch;
    }

    return 0;
}

int register_debugger(Ring_VirtualMachine* rvm, RVM_DebugConfig* debug_config) {
    rvm->debug_config = debug_config;
    return 0;
}