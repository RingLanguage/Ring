#include "ring.hpp"
#include <cstdio>
#include <cstring>
#include <string>

// B| 为加粗开始  |B 为加粗结束
std::string command_help_message = R"(
    B|Ring Command Usage:|B
    
            ring [options] <command> [arguments]
    
    B|Development Commands:|B
            B|run    <filename>                              |B:compile and run Ring program
            B|runbc  <filename>                              |B:run Ring binary bytecode file

            B|build  <filename>                              |B:only check syntax
            B|dump   <filename>  [-o <output_file>]          |B:dump bytecode detail after compile
            B|undump <bytecode_file>                         |B:undump bytecode file, human readable
            B|rdb    <filename>                              |B:debug interactive

    B|Utility Commands:|B
            B|man    <keyword>                               |B:get prompt of ring by keyword
            B|version                                        |B:get Ring version
            B|help                                           |B:get help message
    
    B|Options:|B
            B|-O1                                            |B:optimize bytecode with level 1
            B|-e, --escape-strings                           |B:escape strings in dump bytecode
    
    )";

std::string env_help_message     = R"(
    
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

std::string get_help_message() {
    return convert_troff_string_2_c_control(command_help_message);
}

std::string help_list =
    "Your can try those keyword:\n"
    "    \033[0;31m ring man var \033[0m         :variable definite\n"
    "    \033[0;31m ring man func \033[0m        :function definite\n"
    "    \033[0;31m ring man global_var \033[0m  :global variable definite\n"
    "    \033[0;31m ring man import \033[0m      :import package\n"
    "    \033[0;31m ring man class  \033[0m      :class definite\n"
    "    \033[0;31m ring man --list \033[0m      :list all man keyword\n"
    "\n";

std::string variable_definite =
    "\033[0;31m General Ring-Language Gramnmer Manual \033[0m\n"
    "\n"

    "\033[0;33m+Name:\033[0m\n"
    "    variable\n"
    "\n"

    "\033[0;33m+Description:\033[0m\n"
    "    How to definite a variable.\n"
    "\n"

    "\033[0;33m+Gramnmer/Usage:\033[0m\n"
    "    var bool/int/double/string <identifier> [ =  initializer];\n"
    "\n"

    "\033[0;33m+Example:\033[0m\n"
    "    var bool bool_value;\n"
    "    var int int_value;\n"
    "    var double double_value;\n"
    "    var string string_value;\n"
    "\n"

    "\033[0;33m+Tip:\033[0m\n"
    "\n";

std::string function_definite =
    "\033[0;31m General Ring-Language Gramnmer Manual \033[0m\n"
    "\n"

    "\033[0;33m+Name:\033[0m\n"
    "    function\n"
    "\n"

    "\033[0;33m+Description:\033[0m\n"
    "    How to definite a function.\n"
    "\n"

    "\033[0;33m+Gramnmer/Usage:\033[0m\n"
    "   function <identifier> (parameter_list) -> (return_value_list) {\n"
    "       code_block;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Example:\033[0m\n"
    "   function doit1() {\n"
    "       fmt::println_string(\"I'm function doit1, go go go!\");\n"
    "   }\n"
    "\n"

    "\033[0;33m+Tip:\033[0m\n"
    "\n";

std::string global_variable_definite =
    "\033[0;31m General Ring-Language Gramnmer Manual \033[0m\n"
    "\n"

    "\033[0;33m+Name:\033[0m\n"
    "    global variable\n"
    "\n"

    "\033[0;33m+Description:\033[0m\n"
    "    How to definite global variable.\n"
    "\n"

    "\033[0;33m+Gramnmer/Usage:\033[0m\n"
    "   global {\n"
    "       var bool/int/double/string <identifier>;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Example:\033[0m\n"
    "   global {\n"
    "       var bool bool_value;\n"
    "       var int int_value;\n"
    "       var double double_value;\n"
    "       var string string_value;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Tip:\033[0m\n"
    "\n";

std::string import =
    "\033[0;31m General Ring-Language Gramnmer Manual \033[0m\n"
    "\n"

    "\033[0;33m+Name:\033[0m\n"
    "    import package\n"
    "\n"

    "\033[0;33m+Description:\033[0m\n"
    "    How to import package in head of Ring source code file.\n"
    "\n"

    "\033[0;33m+Gramnmer/Usage:\033[0m\n"
    "   import {\n"
    "       <identifier>;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Example:\033[0m\n"
    "   import {\n"
    "       fmt;\n"
    "       debug;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Tip:\033[0m\n"
    "\n";

std::string class_definite =
    "\033[0;31m General Ring-Language Gramnmer Manual \033[0m\n"
    "\n"

    "\033[0;33m+Name:\033[0m\n"
    "    class\n"
    "\n"

    "\033[0;33m+Description:\033[0m\n"
    "    How to definite a class type.\n"
    "\n"

    "\033[0;33m+Gramnmer/Usage:\033[0m\n"
    "   typedef class <identifier> {\n"
    "       <field_list>;\n"
    "       <method_list>;\n"
    "   }\n"
    "\n"

    "\033[0;33m+Example:\033[0m\n"
    "   typedef class Job {\n"
    "       field bool    Running;\n"
    "       field int     JobID;\n"
    "       field double  Score;\n"
    "   \n"
    "       @constructor\n"
    "       method Job(var bool _Running, var int _JobID, var double _Score);\n"
    "   \n"
    "       method PrintInfo();\n"
    "   }\n"
    "\n"


    "\033[0;33m+Tip:\033[0m\n"
    "\n";

std::string get_man_help(const char* keyword) {
    std::string res = help_list;
    if (str_eq(keyword, "var")) {
        res = variable_definite;
    } else if (str_eq(keyword, "func")) {
        res = function_definite;
    } else if (str_eq(keyword, "global_var")) {
        res = global_variable_definite;
    } else if (str_eq(keyword, "import")) {
        res = import;
    } else if (str_eq(keyword, "class")) {
        res = class_definite;
    } else if (str_eq(keyword, "--list")) {
        res = help_list;
    }
    return res;
}
