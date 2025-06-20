#include "clipp.h"
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


// B| 为加粗开始  |B 为加粗结束
std::string command_help_message = R"(
B|Ring Command Usage:|B

        ring [options] <command> [arguments]

B|All Commands:|B

        B|run    <filename>                              |B:compile and run Ring program
        B|build  <filename>                              |B:only check syntax
        B|dump   <filename>                              |B:dump bytecode detail after compile
        B|rdb    <filename>                              |B:debug interactive

        B|man    <keyword>                               |B:get prompt of ring by keyword
        B|version                                        |B:get Ring version
        B|help                                           |B:get Ring version

B|Options:|B

        B|-O1                                            |B:optimize bytecode with level 1


B|Ring Debug Environment Usage:|B

        B|RING_DEBUG=<debug_value>                       |B:enable various debugging facilities.

        <debug_value> are available:
            trace_func_backtrace=1
                    Enable trace function backtrace
            trace_coroutine_sched=1
                    Enable trace coroutine scheduler
            trace_closure_free_value=1
                    Enable trace closure free value

        <debug_value> also can hold a comma-separated list of these settings:
                  trace_func_backtrace=1,trace_coroutine_sched=1


        e.g. RING_DEBUG=trace_func_backtrace=1,trace_coroutine_sched=1,trace_closure_free_value=1 ring run test.ring

)";


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

    // printf("cmd:%d\n", cmd);
    // printf("input_file_name:%s\n", input_file_name.c_str());
    // printf("keyword:%s\n", keyword.c_str());
    // printf("optimize_level:%d\n", optimize_level);
    // printf("rdb_interpreter:%s\n", rdb_interpreter.c_str());
    // printf("shell_args:\n");
    // for (int i = 0; i < shell_args.size(); i++) {
    //     printf("[i]:%s\n", shell_args[i].c_str());
    // }

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

ExecuterEntry* ring_compile_main(Ring_Command_Arg command_arg) {

    /*
     *初始化编译阶段的 Memory Pool
     *
     * 在编译完成之后, destory_front_mem_pool
     * 解耦编译器前后端
     */
    init_front_mem_pool();

    /*
     * 初始化语法处理节点相关的struct
     */
    CompilerEntry* compiler_entry = compiler_entry_create();
    // TODO: 目前main package 只能有一个源文件
    // main package 源文件即为 ring run 指定的输入文件
    Package* main_package = package_create_input_file(compiler_entry,
                                                      (char*)PACKAGE_MAIN,
                                                      (char*)command_arg.input_file_name.c_str());
    // 将 shell_args 注册到 main-package 中
    // 在 main函数中可以通过这种方式获取:
    // func main(var string[] args) { fmt::println(args); }
    main_package->shell_args = command_arg.shell_args;
    //
    compiler_entry->main_package = main_package;

    /*
     * 初始化代码生成阶段相关的struct
     */
    ExecuterEntry*    executer_entry        = executer_entry_create();
    Package_Executer* main_package_executer = package_executer_create(executer_entry, (char*)PACKAGE_MAIN);
    //
    executer_entry->main_package_executer = main_package_executer;

    // Step-0: 预编译官方std包, 并生成vmcode
    compile_std_lib(compiler_entry, executer_entry);

    // Step-1: flex 词法分析，
    // Step-2: bison 语法分析，构建语法树
    // Step-3: 修正语法树
    package_compile(main_package);

    // Step-4: 生成虚拟机中间代码
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

int run_main(Ring_Command_Arg command_arg) {
    ExecuterEntry*       executer_entry = nullptr;
    Ring_VirtualMachine* rvm            = nullptr;

    executer_entry                      = ring_compile_main(command_arg);
    rvm                                 = ring_virtualmachine_create();

    return ring_exec(rvm, executer_entry);
}


int build_main(Ring_Command_Arg command_arg) {
    ring_compile_main(command_arg);
    return 0;
}

int dump_main(Ring_Command_Arg command_arg) {
    ExecuterEntry* executer_entry = nullptr;

    executer_entry                = ring_compile_main(command_arg);
    // Only dump `main` package bytecode detail.
    package_executer_dump(executer_entry->main_package_executer);
    return 0;
}

int rdb_main(Ring_Command_Arg command_arg) {
    printf(LOG_COLOR_YELLOW);
    printf("%s\n", RING_VERSION);
    printf("\n");
    printf("Start Ring Debugger...\n");
    printf("\n");
    printf("Input file:%s\n", command_arg.input_file_name.c_str());
    printf("Interpreter:%s\n", command_arg.rdb_interpreter.c_str());
    printf("\n");
    printf(LOG_COLOR_CLEAR);

    int                  exit_code      = 0;
    ExecuterEntry*       executer_entry = nullptr;
    Ring_VirtualMachine* rvm            = nullptr;
    RVM_DebugConfig*     debug_config   = nullptr;

    debug_config                        = new_debug_config(command_arg);

PROCESS_NEW_SESSION:

    // 处理 rdb 命令
    RDB_Arg rdb_arg;
    rdb_arg = cli_rdb_message_process_loop_norun(debug_config);
    if (rdb_arg.cmd == RDB_COMMAND_RUN) {
        std::string file = rdb_arg.argument;
        // rdb_arg 转化为 ring_command_arg

        if (file.size()) {
            command_arg.input_file_name = file;
        }
    }


    // 编译
    executer_entry = ring_compile_main(command_arg);
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

int version_main(Ring_Command_Arg command_arg) {
    fprintf(stdout, "%s\n", RING_VERSION);
    return 0;
}

int man_main(Ring_Command_Arg command_arg) {
    std::string tmp;
    tmp = get_man_help(command_arg.keyword.c_str());
    fprintf(stdout, "%s", tmp.c_str());
    return 0;
}

int help_main(Ring_Command_Arg command_arg) {
    std::string tmp;
    tmp = convert_troff_string_2_c_control(command_help_message);
    fprintf(stdout, "%s", tmp.c_str());
    return 0;
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
        exit_code = run_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_BUILD) {
        exit_code = build_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_DUMP) {
        exit_code = dump_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_RDB) {
        exit_code = rdb_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_VERSION) {
        exit_code = version_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_MAN) {
        exit_code = man_main(command_arg);
    } else if (command_arg.cmd == RING_COMMAND_HELP) {
        exit_code = help_main(command_arg);
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