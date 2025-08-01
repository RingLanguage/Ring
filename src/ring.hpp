
#ifndef RING_INCLUDE_H
#define RING_INCLUDE_H

#include "dap.hpp"
#include "linenoise.h"
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define RING_VERSION "ring-v0.3.0-beta Copyright (C) 2021-2025 ring.wiki, ZhenhuLi"

using json = nlohmann::json;


typedef struct Ring_Command_Arg             Ring_Command_Arg;
typedef struct Ring_VirtualMachine          Ring_VirtualMachine;
typedef struct RingCoroutine                RingCoroutine;
typedef struct GarbageCollector             GarbageCollector;
typedef struct ImportPackageInfo            ImportPackageInfo;
typedef struct RingContext                  RingContext;
typedef struct CompilerEntry                CompilerEntry;
typedef struct ExecuterEntry                ExecuterEntry;
typedef struct Package_Executer             Package_Executer;
typedef struct Package                      Package;
typedef struct PackageUnit                  PackageUnit;
typedef struct RingFileStat                 RingFileStat;
typedef struct SourceLineInfo               SourceLineInfo;
typedef struct Location                     Location;

typedef struct RVM_Variable                 RVM_Variable;
typedef struct RVM_RuntimeStack             RVM_RuntimeStack;
typedef struct RVM_RuntimeStatic            RVM_RuntimeStatic;
typedef struct RVM_RuntimeHeap              RVM_RuntimeHeap;
typedef struct RVM_OpcodeBuffer             RVM_OpcodeBuffer;
typedef struct RVM_Opcode_Info              RVM_Opcode_Info;

typedef struct RVM_LabelTable               RVM_LabelTable;
typedef struct RVM_SourceCodeLineMap        RVM_SourceCodeLineMap;
typedef struct Ring_String                  Ring_String;
typedef struct Ring_BasicValue              Ring_BasicValue;
typedef struct Ring_Array                   Ring_Array;

typedef struct Ring_Buildin_Func            Ring_Buildin_Func;
typedef struct EnumDeclaration              EnumDeclaration;
typedef struct EnumItemDeclaration          EnumItemDeclaration;
typedef struct ClassDefinition              ClassDefinition;
typedef struct ClassMemberDeclaration       ClassMemberDeclaration;
typedef struct FieldMember                  FieldMember;
typedef struct MethodMember                 MethodMember;
typedef struct Statement                    Statement;
typedef struct StatementExecResult          StatementExecResult;
typedef struct Expression                   Expression;
typedef struct ArrayIndexExpression         ArrayIndexExpression;
typedef struct NewArrayExpression           NewArrayExpression;
typedef struct ArrayLiteralExpression       ArrayLiteralExpression;
typedef struct ClassObjectLiteralExpression ClassObjectLiteralExpression;
typedef struct CastExpression               CastExpression;
typedef struct KeyExpression                KeyExpression;
typedef struct MemberExpression             MemberExpression;
typedef struct DimensionExpression          DimensionExpression;
typedef struct SubDimensionExpression       SubDimensionExpression;
typedef struct SliceExpression              SliceExpression;
typedef struct SubSliceExpression           SubSliceExpression;
typedef struct RangeExpression              RangeExpression;
typedef struct SubStepRangeExpression       SubStepRangeExpression;
typedef struct SubLinearRangeExpression     SubLinearRangeExpression;
typedef struct BinaryExpression             BinaryExpression;
typedef struct TernaryExpression            TernaryExpression;
typedef struct FunctionCallExpression       FunctionCallExpression;
typedef struct MemberCallExpression         MemberCallExpression;
typedef struct AssignExpression             AssignExpression;
typedef struct FieldInitExpression          FieldInitExpression;
typedef struct LaunchExpression             LaunchExpression;
typedef struct AnonymousFuncExpression      AnonymousFuncExpression;
typedef struct ImmediateInvokFuncExpression ImmediateInvokFuncExpression;
typedef struct IdentifierExpression         IdentifierExpression;

typedef struct ArgumentList                 ArgumentList;
typedef struct Parameter                    Parameter;
typedef struct Identifier                   Identifier;
typedef struct FunctionTuple                FunctionTuple;
typedef struct AnonymousFunc                AnonymousFunc;
typedef struct FreeValueDesc                FreeValueDesc;
typedef struct Function                     Function;
typedef struct Block                        Block;
typedef struct FunctionReturnList           FunctionReturnList;

typedef struct IfStatement                  IfStatement;
typedef struct ElseIfStatement              ElseIfStatement;
typedef struct ForStatement                 ForStatement;
typedef struct ForTernaryStatement          ForTernaryStatement;
typedef struct ForRangeStatement            ForRangeStatement;
typedef struct DoForStatement               DoForStatement;
typedef struct BreakStatement               BreakStatement;
typedef struct ReturnStatement              ReturnStatement;
typedef struct ContinueStatement            ContinueStatement;

typedef struct Ring_DeriveType_Array        Ring_DeriveType_Array;
typedef struct Ring_DeriveType_Class        Ring_DeriveType_Class;
typedef struct Ring_DeriveType_Func         Ring_DeriveType_Func;
typedef struct VarDecl                      VarDecl;
typedef struct Variable                     Variable;
typedef struct TagDefinitionStatement       TagDefinitionStatement;
typedef struct JumpTagStatement             JumpTagStatement;
typedef struct DeferStatement               DeferStatement;
typedef struct TypeAlias                    TypeAlias;
typedef struct TypeSpecifier                TypeSpecifier;
typedef struct StdPackageNativeFunction     StdPackageNativeFunction;

typedef struct StdPackageInfo               StdPackageInfo;
typedef struct RDB_Command                  RDB_Command;
typedef struct RVM_DebugConfig              RVM_DebugConfig;
typedef struct RVM_BreakPoint               RVM_BreakPoint;
typedef struct RVM_Frame                    RVM_Frame;
typedef struct ErrorMessageInfo             ErrorMessageInfo;
typedef struct ErrorReportContext           ErrorReportContext;

typedef struct AttributeInfo                AttributeInfo;

typedef struct RVM_ConstantPool             RVM_ConstantPool;
typedef struct RVM_Constant                 RVM_Constant;
typedef struct RVM_String                   RVM_String;
typedef struct RVM_Array                    RVM_Array;
typedef struct RVM_Closure                  RVM_Closure;
typedef struct RVM_FreeValueDesc            RVM_FreeValueDesc;
typedef struct RVM_FreeValue                RVM_FreeValue;
typedef struct RVM_FreeValueBlock           RVM_FreeValueBlock;
typedef struct RVM_ClassObject              RVM_ClassObject;
typedef struct RVM_GC_Object                RVM_GC_Object;
typedef struct RVM_TypeSpecifier_Class      RVM_TypeSpecifier_Class;
typedef struct RVM_TypeSpecifier_Array      RVM_TypeSpecifier_Array;
typedef struct RVM_TypeSpecifier_Func       RVM_TypeSpecifier_Func;
typedef struct RVM_TypeSpecifier            RVM_TypeSpecifier;

typedef struct RVM_Parameter                RVM_Parameter;
typedef struct RVM_ReturnValue              RVM_ReturnValue;
typedef struct RVM_LocalVariable            RVM_LocalVariable;

typedef struct NativeFunction               NativeFunction;
typedef struct DeriveFunction               DeriveFunction;
typedef struct RVM_Function                 RVM_Function;
typedef struct RVM_Field                    RVM_Field;
typedef struct RVM_Method                   RVM_Method;
typedef struct RVM_ClassDefinition          RVM_ClassDefinition;
typedef struct RVM_CallInfo                 RVM_CallInfo;
typedef struct CallInfo                     CallInfo;
typedef struct RVM_DeferItem                RVM_DeferItem;

typedef struct RVM_GC_Object                RVM_GC_Object;

typedef unsigned char                       RVM_Byte;

typedef struct MemPool                      MemPool;
typedef struct MemBlock                     MemBlock;

typedef struct Ring_Grammar_Info            Ring_Grammar_Info;

typedef class RVM_RangeIterator             RVM_RangeIterator;


typedef enum {
    GRAMMAR_UNKNOW = 0,
    GRAMMAR_IMPORT_PACKAGE,
    GRAMMAR_FUNCTION_DEFIN,
    GRAMMAR_CLASS_DEFIN,

} RING_GRAMMAR_ID;

struct Ring_Grammar_Info {
    RING_GRAMMAR_ID          id;
    std::vector<std::string> grammar;
};

typedef enum : unsigned char {
    RVM_VALUE_TYPE_UNKNOW,

    RVM_VALUE_TYPE_BOOL,
    RVM_VALUE_TYPE_INT,
    RVM_VALUE_TYPE_INT64,
    RVM_VALUE_TYPE_DOUBLE,
    RVM_VALUE_TYPE_STRING,
    RVM_VALUE_TYPE_CLASS_OB,
    RVM_VALUE_TYPE_ARRAY,
    RVM_VALUE_TYPE_CLOSURE,
    RVM_VALUE_RANGE_ITERATOR,

} RVM_Value_Type;

typedef enum : int {
    RVM_FALSE,
    RVM_TRUE,
} RVM_Bool;

/*
 * 在这里有个问题:
 * 如果没有 RVM_Value.type 的时候, size(RVM_Value)=8,
 * 如果有 RVM_Value.type 的时候, size(RVM_Value)=16,
 * 因为内存对齐的原因导致8字节被浪费了, 这里需要优化一下
 *
 * TIP-1: 为什么 bool_value 的类型为 int
 *        这样在 vm实现字节码的时候, 可以少实现一个, bool_value int_value 比较方法一致
 */
typedef struct {
    RVM_Value_Type type;
    union {
        RVM_Bool           bool_value;
        int                int_value;
        long long          int64_value;
        double             double_value;
        RVM_String*        string_value;
        RVM_ClassObject*   class_ob_value;
        RVM_Array*         array_value;
        RVM_Closure*       closure_value;
        RVM_RangeIterator* range_iterator_value;
        // range_iterator_value 只用在栈上，用于临时存放
    } u;

} RVM_Value;


// Ring 源代码的相关信息
struct RingFileStat {
    std::string dir;           // 文件path的绝对路径
    std::string file_name;     // 文件的名称
    std::string abs_path;      // 文件全路径的绝对路径
    long long   last_modified; // last_modified 时间戳, 后续使用md5

    FILE*       fp; // 随机访问 fp

    // 该文件每行的偏移量，用于快速获取某行的内容
    // line_offset_map[0] 没有意义，因为文件都是从第1行开始的
    // line_offset_map[3] 表示第3行开始的文件字节偏移量 和 字节大小
    // 可通过 fseek() 快速获取某行的内容
    std::vector<SourceLineInfo> line_offset_map;
};

typedef int (*TraceDispacth)(RVM_Frame* frame, const char* event, const char* arg);

struct Ring_VirtualMachine {
    Package_Executer*    executer;
    ExecuterEntry*       executer_entry;

    RVM_RuntimeStatic*   runtime_static;
    RVM_RuntimeHeap*     runtime_heap;

    RVM_ClassDefinition* class_list;
    unsigned int         class_size;

    MemPool*             meta_pool;
    MemPool*             data_pool;

    RVM_DebugConfig*     debug_config;

    RingCoroutine*       current_coroutine;
};

// 从后边获取 1BYTE的操作数
#define OPCODE_GET_1BYTE(p) \
    (((p)[0]))
// 从后边获取 2BYTE的操作数
#define OPCODE_GET_2BYTE(p) \
    (((p)[0] << 8) + (p)[1])
// 把两BYTE的操作数放到后边
#define OPCODE_SET_2BYTE(p, value) \
    (((p)[0] = (value) >> 8), ((p)[1] = value & 0xff))


#define VM_STATIC_DATA (rvm->runtime_static->data)


// virtual machine current coroutine
#define VM_CUR_CO (rvm->current_coroutine)

// virtual machine current coroutine's runtime_stack
#define VM_CUR_CO_STACK (rvm->current_coroutine->runtime_stack)
#define VM_CUR_CO_STACK_DATA (rvm->current_coroutine->runtime_stack->data)
#define VM_CUR_CO_STACK_TOP_INDEX (rvm->current_coroutine->runtime_stack->top_index)

// virtual machine current coroutine's call_info
#define VM_CUR_CO_CALLINFO (rvm->current_coroutine->call_info)

// virtual machine current coroutine's code/pc
#define VM_CUR_CO_CODE_LIST (rvm->current_coroutine->call_info->code_list)
#define VM_CUR_CO_CODE_SIZE (rvm->current_coroutine->call_info->code_size)
#define VM_CUR_CO_PC (rvm->current_coroutine->call_info->pc)


#define VM_CUR_CO_CSB (rvm->current_coroutine->call_info->caller_stack_base)

typedef enum {
    CO_STAT_INIT,
    CO_STAT_RUNNING,
    CO_STAT_SUSPENDED,
    CO_STAT_DEAD,
} CO_STAT;

typedef unsigned long long CO_ID;


/*
 * RingCoroutine 为一个用户协程
 * 用户通过 launch() 创建一个协程
 *
 * co_id:          协程唯一ID
 * p_co_id:        父协程ID, 谁唤醒它，他就是它的父协程
 * last_run_time:  上次运行时间, ns
 * status:         协程状态
 *
 * runtime_stack: 运行栈，该协程的独占的栈空间
 *
 * call_info:     函数调用栈
 *
 */
struct RingCoroutine {
    CO_ID             co_id;
    CO_ID             p_co_id;

    long long         last_run_time;
    CO_STAT           status;

    RVM_RuntimeStack* runtime_stack; // 运行堆栈

    RVM_CallInfo*     call_info; // 函数调用栈

    // defer 调用链
    unsigned int   defer_list_size;
    RVM_DeferItem* defer_list;
};

struct GarbageCollector {
};

struct ImportPackageInfo {
    unsigned int line_number;

    char*        package_name;
    char*        package_path;
    char*        rename;
    std::string  package_md5;

    /*
     * package_name:
     *     用户 import : 是包含远程路径的/也可以是一个标准包/也可以是个本项目包,
     *     e.g. github.com/G/P
     *     主要通过这个来裁定是否重复编译了
     *
     * package_path:
     *     可以 重定向到本地 Pakcage 的路径
     *
     * rename:
     *     多个package 可能有重复, 支持用户重命名 package
     *
     * package_md5:
     *     用于链接使用, 主要为编译和链接 生成一个唯一ID
     */
};

typedef enum : unsigned char {
    OPTIMIZE_LEVEL_NONE = 0,
    OPTIMIZE_LEVEL_1, // 开启常量折叠，简单的死代码消除
} OPTIMIZE_LEVEL;

struct RingContext {
    Package*       package;
    PackageUnit*   package_unit;
    FunctionTuple* func;
    Block*         block;
    Statement*     statement;
    Expression*    expression;

    unsigned int   optimize_level;
};

struct CompilerEntry {
    // std::unordered_map<std::string, Package*> package_map;
    std::vector<Package*> package_list;
    Package*              main_package;
};

struct ExecuterEntry {
    std::vector<Package_Executer*> package_executer_list;
    Package_Executer*              main_package_executer;
};

struct Package_Executer {
    ExecuterEntry*       executer_entry;
    unsigned int         package_index; // 在 ExecuterEntry 中的 index
    char*                package_name;

    RVM_ConstantPool*    constant_pool;

    unsigned int         global_variable_size;
    RVM_Variable*        global_variable_list;

    unsigned int         function_size;
    RVM_Function*        function_list;

    unsigned int         class_size;
    RVM_ClassDefinition* class_list;

    unsigned int         bootloader_code_size;
    RVM_Byte*            bootloader_code_list;
    unsigned int         bootloader_need_stack_size; // 需要的栈空间

    bool                 exist_main_func;
    unsigned int         main_func_index;
    bool                 exist_global_init_func;
    unsigned int         global_init_func_index;

    unsigned int         estimate_runtime_stack_capacity;
};

/*
 * PackageUnit 与 Package 的逻辑关系
 *
 * 一个PackageUnit 对应一个 Ring 源码文件
 * 每个 PackageUnit 都有一个对应的 Package
 * 一个 Package 可以有多个 PackageUnit
 */
struct Package {
    CompilerEntry*           compiler_entry;
    unsigned int             package_index; // 在 CompilerEntry 中的 index
    char*                    package_name;
    char*                    package_path;

    std::vector<std::string> source_file_list;

    // 当前 一个 package中只能有一个源文件，所以当前的设计是符合要求的
    // 如果后续 一个packag中含有多个源文件，可能得需要这样设计：
    // std::vecotr<<std::pair<unsigned int, Statment*>>  global_decl_list;
    // 代表了 该package下所有的 global变量，分布在 多个 global{} 块中。
    // pair.first 是 size
    // pair.second 就是 global_statement_list
    std::vector<std::pair<unsigned int, Statement*>>  global_block_statement_list;
    std::vector<VarDecl*>                             global_var_decl_list;

    std::vector<ClassDefinition*>                     class_definition_list;
    std::vector<Function*>                            function_list;
    std::vector<TypeAlias*>                           type_alias_list;

    std::unordered_set<std::string>                   global_identifier_map;
    std::unordered_map<std::string, VarDecl*>         global_var_decl_map;
    std::unordered_map<std::string, ClassDefinition*> class_definition_map;
    std::unordered_map<std::string, Function*>        function_map;
    std::unordered_map<std::string, std::string>      import_package_map;

    std::vector<PackageUnit*>                         package_unit_list;

    unsigned int                                      compile_error_num;

    std::vector<std::string>                          shell_args;
    // shell_args 只有 main-package 才会用到
    // e.g.  ./bin/ring run ./test.ring args1 args2
    // shell_args 就是 [args1, args2]
};

struct SourceLineInfo {
    off_t        start_offset; // 某行相对于文件开始的偏移
    unsigned int size;         // 某行的字符数量
};

struct Location {
    unsigned int line_number;
};

/*
 * PackageUnit 与 Package 的逻辑关系
 *
 * 一个PackageUnit 对应一个 Ring 源码文件
 * 每个 PackageUnit 都有一个对应的 Package
 * 一个 Package 可以有多个 PackageUnit
 */
struct PackageUnit {
    Package*      parent_package;

    RingFileStat* ring_file_stat; // ring源代码文件信息
    std::string   current_file_name;
    FILE*         current_file_fp; // 这个 FILE* 只能给 bison使用
    FILE*         file_fp_random;  // 用来随机fseek并获取文件内容

    unsigned int  current_line_number;
    unsigned int  current_column_number;
    Ring_String*  current_line_content;

    // 该文件每行的偏移量，用于快速获取某行的内容
    // line_offset_map[0] 没有意义，因为文件都是从第1行开始的
    // line_offset_map[3] 表示第3行开始的文件字节偏移量 和 字节大小
    // 可通过 fseek() 快速获取某行的内容
    off_t                           current_line_start_offset;
    off_t                           current_offset;
    std::vector<SourceLineInfo>     line_offset_map;

    std::vector<ImportPackageInfo*> import_package_list;

    unsigned int                    global_block_statement_list_size;
    Statement*                      global_block_statement_list;

    std::vector<ClassDefinition*>   class_definition_list;
    std::vector<Function*>          function_list;
    std::vector<EnumDeclaration*>   enum_declaration_list; // TODO: 删除
    std::vector<TypeAlias*>         type_alias_list;
    // TODO: 删除 class_definition_list ，统一通过 type_alias_list 来代替

    Block*       current_block;

    unsigned int compile_error_num;
};


/*
 * TypeSpecifier
 *
 * 基础类型：
 *     - bool
 *     - int
 *     - double
 * 对象类型：
 *     - string
 *     - array
 *     - class
 * 范型推导
 *     - any
 *
 */
typedef enum {
    RING_BASIC_TYPE_UNKNOW,

    RING_BASIC_TYPE_BOOL,
    RING_BASIC_TYPE_INT,
    RING_BASIC_TYPE_INT64,
    RING_BASIC_TYPE_DOUBLE,
    RING_BASIC_TYPE_STRING,

    RING_BASIC_TYPE_CLASS,
    RING_BASIC_TYPE_ARRAY,
    RING_BASIC_TYPE_FUNC,

    RING_BASIC_TYPE_ANY,

} Ring_BasicType;


struct Ring_DeriveType_Array {
    unsigned int   dimension;
    TypeSpecifier* sub;
};

struct Ring_DeriveType_Class {
    Package*         package; // 定义class 所在的package
    char*            class_identifier;
    ClassDefinition* class_definition; // UPDATED_BY_FIX_AST
};

// 函数变量类型
struct Ring_DeriveType_Func {
    unsigned int        parameter_list_size;
    Parameter*          parameter_list;
    unsigned int        return_list_size;
    FunctionReturnList* return_list;
};


// TODO: 应该分成这几种类型
// typedef 定义的类型别名，需要递归查找
// typedef 定义的类定义
// teypdef 定义的函数别名
// typedef 定义的枚举类型
struct TypeAlias {
    unsigned int     line_number;

    char*            identifier;
    TypeSpecifier*   type_specifier;

    bool             is_enum;
    EnumDeclaration* enum_declaration;
};

// Only used by front-end of compiler.
struct TypeSpecifier {
    unsigned int line_number;

    char*        package_posit; // 空表示当前package
    // 只有非 bool/int/int64/double/string 时, identifier才会有效
    // 如类, 函数别名
    char*          identifier;

    Ring_BasicType kind;
    union {
        Ring_DeriveType_Array* array_t;
        Ring_DeriveType_Class* class_t;
        Ring_DeriveType_Func*  func_t;
    } u;
};

#define TYPE_IS_STRING_ARRAY_1(type_specifier)         \
    (((type_specifier)->kind == RING_BASIC_TYPE_ARRAY) \
     && ((type_specifier)->u.array_t->dimension == 1)  \
     && ((type_specifier)->u.array_t->sub != nullptr)  \
     && ((type_specifier)->u.array_t->sub->kind == RING_BASIC_TYPE_STRING))

#define TYPE_IS_INT32OR64(type)            \
    (((type)->kind == RING_BASIC_TYPE_INT) \
     || ((type)->kind == RING_BASIC_TYPE_INT64))

#define TYPE_IS_NUM(type)                       \
    (((type)->kind == RING_BASIC_TYPE_INT)      \
     || ((type)->kind == RING_BASIC_TYPE_INT64) \
     || ((type)->kind == RING_BASIC_TYPE_DOUBLE))

#define TYPE_IS_BASIC(type)                      \
    (((type)->kind == RING_BASIC_TYPE_BOOL)      \
     || ((type)->kind == RING_BASIC_TYPE_INT)    \
     || ((type)->kind == RING_BASIC_TYPE_INT64)  \
     || ((type)->kind == RING_BASIC_TYPE_DOUBLE) \
     || ((type)->kind == RING_BASIC_TYPE_STRING))

#define TYPE_IS_BOOL(type) \
    (((type)->kind == RING_BASIC_TYPE_BOOL))
#define TYPE_IS_INT(type) \
    (((type)->kind == RING_BASIC_TYPE_INT))
#define TYPE_IS_INT64(type) \
    (((type)->kind == RING_BASIC_TYPE_INT64))
#define TYPE_IS_DOUBLE(type) \
    (((type)->kind == RING_BASIC_TYPE_DOUBLE))
#define TYPE_IS_STRING(type) \
    (((type)->kind == RING_BASIC_TYPE_STRING))
#define TYPE_IS_ARRAY(type) \
    (((type)->kind == RING_BASIC_TYPE_ARRAY))

// 比较运算符 > >= < <= == !=
#define TYPE_IS_COMPARE_EQ(type)                 \
    (((type)->kind == RING_BASIC_TYPE_BOOL)      \
     || ((type)->kind == RING_BASIC_TYPE_INT)    \
     || ((type)->kind == RING_BASIC_TYPE_INT64)  \
     || ((type)->kind == RING_BASIC_TYPE_DOUBLE) \
     || ((type)->kind == RING_BASIC_TYPE_STRING))

// 比较运算符 > >= < <=
#define TYPE_IS_COMPARE_REL(type)                \
    (((type)->kind == RING_BASIC_TYPE_INT)       \
     || ((type)->kind == RING_BASIC_TYPE_INT64)  \
     || ((type)->kind == RING_BASIC_TYPE_DOUBLE) \
     || ((type)->kind == RING_BASIC_TYPE_STRING))


typedef void RVM_NativeFuncProc(Ring_VirtualMachine* rvm,
                                unsigned int arg_size, RVM_Value* args,
                                unsigned int* return_size, RVM_Value** return_list);

typedef enum {
    RVM_FUNCTION_TYPE_UNKNOW,
    RVM_FUNCTION_TYPE_NATIVE, // 原生函数
    RVM_FUNCTION_TYPE_DERIVE, // 派生函数，库函数，Ring编写的库函数
} RVMFunctionType;

struct RVM_Parameter {
    RVM_TypeSpecifier* type_specifier;
    bool               is_variadic; // variadic parameter function can be called with any number of trailing arguments.
    char*              identifier;
};

struct RVM_ReturnValue {
    RVM_TypeSpecifier* type_specifier;
};

struct RVM_LocalVariable {
    RVM_TypeSpecifier* type_specifier;
    char*              identifier;
};

struct NativeFunction {
    RVM_NativeFuncProc* func_proc;
    int                 arg_count;         // -1 表示可变参数
    int                 return_list_count; // 返回值的数量
};
struct DeriveFunction {
    unsigned int                       code_size;
    RVM_Byte*                          code_list;
    unsigned int                       need_stack_size;

    std::vector<RVM_SourceCodeLineMap> code_line_map;
    // 一行Ring源代码 对应 一个 RVM_SourceCodeLineMap
    // 空行、注释行 没有对应的 RVM_SourceCodeLineMap
    // RVM_SourceCodeLineMap.opcode_begin_index 是单调递增的
};


struct RVM_Function {
    std::string        source_file;
    unsigned int       start_line_number;
    unsigned int       end_line_number;
    RingFileStat*      ring_file_stat;

    unsigned int       parameter_size;
    RVM_Parameter*     parameter_list;
    unsigned int       return_value_size;
    RVM_ReturnValue*   return_value_list;
    unsigned int       local_variable_size;
    RVM_LocalVariable* local_variable_list;
    unsigned int       free_value_size;
    RVM_FreeValueDesc* free_value_list;

    unsigned int       estimate_runtime_stack_capacity;

    char*              identifier;
    RVMFunctionType    type;
    union {
        NativeFunction* native_func;
        DeriveFunction* derive_func;
    } u;
};


struct RVM_FreeValueDesc {
    const char* identifier;
    bool        is_curr_local;
    union {
        unsigned int curr_local_index;     // 本函数定义的局部变量, 那么本函数return的时候, 需要负责关闭相关的局部变量
        unsigned int out_free_value_index; // 是外层函数的FreeValue，还得递归向上找，直到找到 curr_local_index
    } u;
    // curr_local_index function 局部变量的 stack-index
};

typedef enum {
    RVM_FREEVALUE_STATE_UNKNOW,
    RVM_FREEVALUE_STATE_OPEN,
    RVM_FREEVALUE_STATE_CLOSE,
    RVM_FREEVALUE_STATE_RECUR,
} RVM_FreeValue_State;

struct RVM_FreeValue {
    RVM_FreeValue_State state;

    union {
        RVM_Value*     p;     // state == open | close
        RVM_FreeValue* recur; // state == recur
        // p 可指向 open/close value
        //     open 时，p指向栈空间
        //     close 时，p指向c_value，是一个拷贝
    } u;

    RVM_Value c_value; // closed value

    // state == open | close 以下才有用
    RVM_Closure* belong_closure; // free_value 所在的 closure
};


struct RVM_Field {
    char*              identifier;
    RVM_TypeSpecifier* type_specifier;
};

// 他的本质就是一个 函数
// 只不过他可以引用 field
// TODO: RVM_Method 需要简化一下, 跟 RVM_Function其实是一样的
struct RVM_Method {
    char*         identifier;
    RVM_Function* rvm_function;
};

struct RVM_ClassDefinition {
    std::string  source_file;
    unsigned int start_line_number; // 源码的开始行
    unsigned int end_line_number;   // 源码的结束行

    char*        identifier;

    unsigned int class_index; // TODO: 这里是相对于哪里的偏移量，是整个字节码么，还是一个package 内的

    unsigned int field_size;
    RVM_Field*   field_list;

    unsigned int method_size;
    RVM_Method*  method_list;
};

#define CLASS_GET_FIELD_IDENT(rvm_class_def, field_index) \
    ((rvm_class_def)->field_list[(field_index)].identifier)


typedef enum {
    CONSTANTPOOL_TYPE_UNKNOW,
    CONSTANTPOOL_TYPE_INT,
    CONSTANTPOOL_TYPE_INT64,
    CONSTANTPOOL_TYPE_DOUBLE,
    CONSTANTPOOL_TYPE_STRING,
    CONSTANTPOOL_TYPE_CLOSURE,
} ConstantPoolType;

struct RVM_ConstantPool {
    unsigned int  size;
    RVM_Constant* list;

    // 字符串常量需要去重
    std::unordered_map<std::string, int> string_index_map;
};

struct RVM_Constant {
    ConstantPoolType type;
    union {
        int           int_value;
        long long     int64_value;
        double        double_value;
        RVM_String*   string_value; // 这里不能分配在 heap上
        RVM_Function* anonymous_func_value;
    } u;
};

struct RVM_Variable {
    char*              identifier;
    RVM_TypeSpecifier* type_specifier;
};


typedef enum {
    RVM_OBJECT_TYPE_UNKNOW,
    RVM_GC_OBJECT_TYPE_STRING,
    RVM_GC_OBJECT_TYPE_CLASS_OB,
    RVM_GC_OBJECT_TYPE_ARRAY,
    RVM_GC_OBJECT_TYPE_CLOSURE,
    RVM_GC_OBJECT_TYPE_FVB,
} RVM_GC_Object_Type;

typedef enum {
    GC_MARK_COLOR_UNKNOW,
    GC_MARK_COLOR_WHITE, // 需要被回收
    GC_MARK_COLOR_GRAY,
    GC_MARK_COLOR_BLACK, // 不需要被回收
} GC_Mark;


#define RVM_GC_Object_Header    \
    RVM_GC_Object_Type gc_type; \
    GC_Mark            gc_mark; \
    RVM_GC_Object*     gc_prev; \
    RVM_GC_Object*     gc_next;

struct RVM_GC_Object {
    RVM_GC_Object_Header;
};


struct RVM_String {
    RVM_GC_Object_Header;

    char*        data;
    unsigned int length;
    unsigned int capacity;
};

typedef enum {
    RVM_ARRAY_UNKNOW,
    RVM_ARRAY_BOOL,         // bool 数组
    RVM_ARRAY_INT,          // int 数组
    RVM_ARRAY_INT64,        // int64 数组
    RVM_ARRAY_DOUBLE,       // double 数组
    RVM_ARRAY_STRING,       // string 数组
    RVM_ARRAY_CLASS_OBJECT, // 类 数组
    RVM_ARRAY_A,            // 多维数组的中间态， 感觉有必要删除
    RVM_ARRAY_CLOSURE,      // 匿名函数 数组
} RVM_Array_Type;
// 这里的顺序要和 Ring_BasicType 保持一致


struct RVM_Array {
    RVM_GC_Object_Header;

    RVM_Array_Type type; // 用来选择 u
    unsigned char  dimension;
    unsigned int   length;
    unsigned int   capacity;

    // 数组最终元素的 类型
    // item_type_kind 不可能 为 array
    // 方便快速定位到最终的数组元素，但是当前设计存在冗余
    // TODO: 后续应该废弃掉 RVM_Array_Type type
    Ring_BasicType item_type_kind;

    // 这里实现的不太好，信息冗余了
    // 只有当 type == RVM_ARRAY_CLASS_OBJECT 才有意义
    RVM_ClassDefinition* class_def;

    union {
        bool*             bool_array;
        int*              int_array;
        long long*        int64_array;
        double*           double_array;
        RVM_String**      string_array;
        RVM_ClassObject** class_ob_array;
        RVM_Closure**     closure_array;
        RVM_Array**       a_array; // 多维数组
    } u;
};


struct RVM_ClassObject {
    RVM_GC_Object_Header;

    RVM_ClassDefinition* class_def;
    unsigned int         field_count;
    RVM_Value*           field_list;
};

struct RVM_Closure {
    RVM_GC_Object_Header;

    RVM_Function*       anonymous_func;
    RVM_FreeValueBlock* fvb; // free_value_block
    bool                is_root_closure;
};

struct RVM_FreeValueBlock {
    RVM_GC_Object_Header;

    unsigned int   size;
    RVM_FreeValue* list;
};

struct RVM_TypeSpecifier_Class {
    unsigned int package_index; // 定义 class的package 的全局索引
    unsigned int class_def_index;
};

struct RVM_TypeSpecifier_Array {
    unsigned int       dimension; // 维度，用来指明sub
    RVM_TypeSpecifier* sub;
};

struct RVM_TypeSpecifier_Func {
    unsigned int       parameter_list_size;
    RVM_Parameter*     parameter_list;

    unsigned int       return_list_size;
    RVM_TypeSpecifier* return_list;
};


// Only used by back-end of compiler.
struct RVM_TypeSpecifier {
    Ring_BasicType kind;

    union {
        RVM_TypeSpecifier_Array* array_t;
        RVM_TypeSpecifier_Class* class_t;
        RVM_TypeSpecifier_Func*  func_t;
    } u;
};

// 支持线性寻址
struct RVM_RuntimeStack {
    RVM_Value*   data;
    unsigned int top_index;
    unsigned int size;
    unsigned int capacity;
};

struct RVM_RuntimeStatic {
    RVM_Value*   data;
    unsigned int size;
    unsigned int capacity;
};

struct RVM_RuntimeHeap {
    long long      alloc_size;
    long long      threshold;
    RVM_GC_Object* list;
};

struct RVM_LabelTable {
    char*        label_name;
    unsigned int label_address;
};

struct RVM_SourceCodeLineMap {
    unsigned int line_number;        // 对应Ring源代码文件的行数
    unsigned int opcode_begin_index; // 对应 opcode 的 开始索引
    unsigned int opcode_size;        // 一行Ring源代码 对应 opcode size
};

struct RVM_OpcodeBuffer {
    RVM_Byte*                          code_list;
    unsigned int                       code_size;
    unsigned int                       code_capacity;
    unsigned int                       need_stack_size; // 运行所有字节码需要的最大栈空间

    std::vector<RVM_LabelTable>        lable_list;

    std::vector<RVM_SourceCodeLineMap> code_line_map;
    // 一行Ring源代码 对应 一个 RVM_SourceCodeLineMap
    // 空行、注释行 没有对应的 RVM_SourceCodeLineMap
    // RVM_SourceCodeLineMap.opcode_begin_index 是单调递增的
};

typedef enum {
    OPCODE_OPERAND_TYPE_0BYTE,      // 后边没有操作数, pc+1
    OPCODE_OPERAND_TYPE_1BYTE_A,    // 后边1BYTE操作数, pc+2
    OPCODE_OPERAND_TYPE_2BYTE_As,   // 后边2BYTE操作数 两个字节组合成一个操作数, pc+3
    OPCODE_OPERAND_TYPE_2BYTE_AB,   // 后边2BYTE操作数 两个字节分别为两个不同的操作数, pc+3
    OPCODE_OPERAND_TYPE_3BYTE_AsB,  // 后边3BYTE操作数 第1、2个字节为一个操作数, 第3个字节为一个操作数, pc+4
    OPCODE_OPERAND_TYPE_4BYTE_ABCs, // 后边4BYTE操作数 第1、2个字节各为一个操作数, 第3，4个字节为一个操作数, pc+5
    OPCODE_OPERAND_TYPE_5BYTE_AsBsC,

} OpcodeOperandType;

typedef enum {
    OPCODE_OPERAND_UNKNOW,
    OPCODE_OPERAND_A,
    OPCODE_OPERAND_As,
    OPCODE_OPERAND_B,
    OPCODE_OPERAND_Bs,
    OPCODE_OPERAND_Cs,
} OpcodeOperand;

struct RVM_Opcode_Info {
    RVM_Byte          code;                  // 字节码枚举
    const char*       name;                  // 字节码字符串
    OpcodeOperandType operand_type;          // 字节码后边的操作数所占字节数量
    const char*       stack_incr_expr;       // 对运行时栈空间的增长 表达式
    int               stack_incr_expr_debug; // TODO: 临时使用，后续删除

    /*
     * 字节码的注释, 放在结构体里边, 用于快速生成文档, 目前暂无使用意义, 后续放在debug控制
     *
     * usage_comment:    简要描述一个字节码相关的操作
     * stack_top_change: 对 runtime_stack 的操作变化
     * math_formula:     通过一个数学公式描述字节码操作
     */
    const char* usage_comment;
    const char* stack_top_change;
    const char* math_formula;
};

typedef struct {
    std::string variable;    // 变量名
    int         coefficient; // 系数
} _StackIncrExprTerm;

typedef enum {
    RVM_CODE_UNKNOW = 0,

    // push int/int64/double/string const to stack
    RVM_CODE_PUSH_BOOL,
    RVM_CODE_PUSH_INT_1BYTE, // operand 0-255
    RVM_CODE_PUSH_INT_2BYTE, // operand 256-65535
    RVM_CODE_PUSH_INT__1,
    RVM_CODE_PUSH_DOUBLE_1,

    RVM_CODE_PUSH_INT,   // bigger 65535
    RVM_CODE_PUSH_INT64, // bigger 65535
    RVM_CODE_PUSH_DOUBLE,
    RVM_CODE_PUSH_STRING,

    // static
    RVM_CODE_POP_STATIC_BOOL,
    RVM_CODE_POP_STATIC_INT,
    RVM_CODE_POP_STATIC_INT64,
    RVM_CODE_POP_STATIC_DOUBLE,
    RVM_CODE_POP_STATIC_STRING,
    RVM_CODE_POP_STATIC_CLASS_OB,
    RVM_CODE_POP_STATIC_ARRAY,
    RVM_CODE_PUSH_STATIC_BOOL,
    RVM_CODE_PUSH_STATIC_INT,
    RVM_CODE_PUSH_STATIC_INT64,
    RVM_CODE_PUSH_STATIC_DOUBLE,
    RVM_CODE_PUSH_STATIC_STRING,
    RVM_CODE_PUSH_STATIC_CLASS_OB,
    RVM_CODE_PUSH_STATIC_ARRAY,


    // stack
    RVM_CODE_POP_STACK_BOOL,
    RVM_CODE_POP_STACK_INT,
    RVM_CODE_POP_STACK_INT64,
    RVM_CODE_POP_STACK_DOUBLE,
    RVM_CODE_POP_STACK_STRING,
    RVM_CODE_POP_STACK_CLASS_OB,
    RVM_CODE_POP_STACK_ARRAY,
    RVM_CODE_POP_STACK_CLOSURE,
    RVM_CODE_PUSH_STACK_BOOL,
    RVM_CODE_PUSH_STACK_INT,
    RVM_CODE_PUSH_STACK_INT64,
    RVM_CODE_PUSH_STACK_DOUBLE,
    RVM_CODE_PUSH_STACK_STRING,
    RVM_CODE_PUSH_STACK_CLASS_OB,
    RVM_CODE_PUSH_STACK_ARRAY,
    RVM_CODE_PUSH_STACK_CLOSURE,

    // free-value
    RVM_CODE_POP_FREE_BOOL,
    RVM_CODE_POP_FREE_INT,
    RVM_CODE_POP_FREE_INT64,
    RVM_CODE_POP_FREE_DOUBLE,
    RVM_CODE_POP_FREE_STRING,
    RVM_CODE_POP_FREE_CLASS_OB,
    RVM_CODE_POP_FREE_ARRAY,
    RVM_CODE_PUSH_FREE_BOOL,
    RVM_CODE_PUSH_FREE_INT,
    RVM_CODE_PUSH_FREE_INT64,
    RVM_CODE_PUSH_FREE_DOUBLE,
    RVM_CODE_PUSH_FREE_STRING,
    RVM_CODE_PUSH_FREE_CLASS_OB,
    RVM_CODE_PUSH_FREE_ARRAY,

    // array
    RVM_CODE_PUSH_ARRAY_A,
    RVM_CODE_PUSH_ARRAY_BOOL,
    RVM_CODE_PUSH_ARRAY_INT,
    RVM_CODE_PUSH_ARRAY_INT64,
    RVM_CODE_PUSH_ARRAY_DOUBLE,
    RVM_CODE_PUSH_ARRAY_STRING,
    RVM_CODE_PUSH_ARRAY_CLASS_OB,
    RVM_CODE_PUSH_ARRAY_CLOSURE,

    RVM_CODE_POP_ARRAY_A,
    RVM_CODE_POP_ARRAY_BOOL,
    RVM_CODE_POP_ARRAY_INT,
    RVM_CODE_POP_ARRAY_INT64,
    RVM_CODE_POP_ARRAY_DOUBLE,
    RVM_CODE_POP_ARRAY_STRING,
    RVM_CODE_POP_ARRAY_CLASS_OB,
    RVM_CODE_POP_ARRAY_CLOSURE,

    // array append
    RVM_CODE_ARRAY_APPEND_A,
    RVM_CODE_ARRAY_APPEND_BOOL,
    RVM_CODE_ARRAY_APPEND_INT,
    RVM_CODE_ARRAY_APPEND_INT64,
    RVM_CODE_ARRAY_APPEND_DOUBLE,
    RVM_CODE_ARRAY_APPEND_STRING,
    RVM_CODE_ARRAY_APPEND_CLASS_OB,
    RVM_CODE_ARRAY_APPEND_CLOSURE,
    // array pop
    RVM_CODE_ARRAY_POP_A,
    RVM_CODE_ARRAY_POP_BOOL,
    RVM_CODE_ARRAY_POP_INT,
    RVM_CODE_ARRAY_POP_INT64,
    RVM_CODE_ARRAY_POP_DOUBLE,
    RVM_CODE_ARRAY_POP_STRING,
    RVM_CODE_ARRAY_POP_CLASS_OB,
    RVM_CODE_ARRAY_POP_CLOSURE,

    // array
    RVM_CODE_NEW_ARRAY_BOOL,
    RVM_CODE_NEW_ARRAY_INT,
    RVM_CODE_NEW_ARRAY_INT64,
    RVM_CODE_NEW_ARRAY_DOUBLE,
    RVM_CODE_NEW_ARRAY_STRING,
    RVM_CODE_NEW_ARRAY_CLASS_OB,
    RVM_CODE_NEW_ARRAY_CLOSURE,

    RVM_CODE_NEW_ARRAY_LITERAL_BOOL,
    RVM_CODE_NEW_ARRAY_LITERAL_INT,
    RVM_CODE_NEW_ARRAY_LITERAL_INT64,
    RVM_CODE_NEW_ARRAY_LITERAL_DOUBLE,
    RVM_CODE_NEW_ARRAY_LITERAL_STRING,
    RVM_CODE_NEW_ARRAY_LITERAL_CLASS_OB,
    RVM_CODE_NEW_ARRAY_LITERAL_CLOSURE,
    RVM_CODE_NEW_ARRAY_LITERAL_A,


    // range array/string
    RVM_CODE_RANGE_STEP_INIT_INT64,
    RVM_CODE_RANGE_STEP_INIT_DOUBLE,
    RVM_CODE_RANGE_LINEAR_INIT,
    RVM_CODE_RANGE_HAS_NEXT,
    RVM_CODE_RANGE_GET_NEXT_1,
    RVM_CODE_RANGE_GET_NEXT_2,
    RVM_CODE_RANGE_FINISH,

    // slice array/string
    RVM_CODE_SLICE_ARRAY,
    RVM_CODE_SLICE_STRING,

    // class
    RVM_CODE_NEW_CLASS_OB_LITERAL,

    RVM_CODE_POP_FIELD_BOOL,
    RVM_CODE_POP_FIELD_INT,
    RVM_CODE_POP_FIELD_INT64,
    RVM_CODE_POP_FIELD_DOUBLE,
    RVM_CODE_POP_FIELD_STRING,
    RVM_CODE_POP_FIELD_CLASS_OB,
    RVM_CODE_POP_FIELD_ARRAY,
    RVM_CODE_POP_FIELD_CLOSURE,

    RVM_CODE_PUSH_FIELD_BOOL,
    RVM_CODE_PUSH_FIELD_INT,
    RVM_CODE_PUSH_FIELD_INT64,
    RVM_CODE_PUSH_FIELD_DOUBLE,
    RVM_CODE_PUSH_FIELD_STRING,
    RVM_CODE_PUSH_FIELD_CLASS_OB,
    RVM_CODE_PUSH_FIELD_ARRAY,
    RVM_CODE_PUSH_FIELD_CLOSURE,

    // arithmetic
    RVM_CODE_ADD_INT,
    RVM_CODE_ADD_INT64,
    RVM_CODE_ADD_DOUBLE,

    RVM_CODE_SUB_INT,
    RVM_CODE_SUB_INT64,
    RVM_CODE_SUB_DOUBLE,

    RVM_CODE_MUL_INT,
    RVM_CODE_MUL_INT64,
    RVM_CODE_MUL_DOUBLE,

    RVM_CODE_DIV_INT,
    RVM_CODE_DIV_INT64,
    RVM_CODE_DIV_DOUBLE,

    RVM_CODE_MOD_INT,
    RVM_CODE_MOD_INT64,
    RVM_CODE_MOD_DOUBLE,

    RVM_CODE_MINUS_INT,
    RVM_CODE_MINUS_INT64,
    RVM_CODE_MINUS_DOUBLE,

    RVM_CODE_SELF_INCREASE_INT,
    RVM_CODE_SELF_INCREASE_INT64,
    RVM_CODE_SELF_INCREASE_DOUBLE,
    RVM_CODE_SELF_DECREASE_INT,
    RVM_CODE_SELF_DECREASE_INT64,
    RVM_CODE_SELF_DECREASE_DOUBLE,

    RVM_CODE_CONCAT,
    RVM_CODE_PUSH_ARRAY_LEN,
    RVM_CODE_PUSH_ARRAY_CAPACITY,
    RVM_CODE_PUSH_STRING_LEN,
    RVM_CODE_PUSH_STRING_CAPACITY,

    // type cast
    RVM_CODE_CAST_BOOL_TO_INT,
    RVM_CODE_CAST_INT_TO_DOUBLE,

    RVM_CODE_CAST_INT_TO_BOOL,
    RVM_CODE_CAST_DOUBLE_TO_INT,

    RVM_CODE_BOOL_2_STRING,
    RVM_CODE_INT_2_STRING,
    RVM_CODE_INT64_2_STRING,
    RVM_CODE_DOUBLE_2_STRING,
    RVM_CODE_INT_2_INT64,

    // logical
    RVM_CODE_LOGICAL_AND,
    RVM_CODE_LOGICAL_OR,
    RVM_CODE_LOGICAL_NOT,

    // relational
    RVM_CODE_RELATIONAL_EQ_INT,
    RVM_CODE_RELATIONAL_EQ_INT64,
    RVM_CODE_RELATIONAL_EQ_DOUBLE,
    RVM_CODE_RELATIONAL_EQ_STRING,

    RVM_CODE_RELATIONAL_NE_INT,
    RVM_CODE_RELATIONAL_NE_INT64,
    RVM_CODE_RELATIONAL_NE_DOUBLE,
    RVM_CODE_RELATIONAL_NE_STRING,

    RVM_CODE_RELATIONAL_GT_INT,
    RVM_CODE_RELATIONAL_GT_INT64,
    RVM_CODE_RELATIONAL_GT_DOUBLE,
    RVM_CODE_RELATIONAL_GT_STRING,

    RVM_CODE_RELATIONAL_GE_INT,
    RVM_CODE_RELATIONAL_GE_INT64,
    RVM_CODE_RELATIONAL_GE_DOUBLE,
    RVM_CODE_RELATIONAL_GE_STRING,

    RVM_CODE_RELATIONAL_LT_INT,
    RVM_CODE_RELATIONAL_LT_INT64,
    RVM_CODE_RELATIONAL_LT_DOUBLE,
    RVM_CODE_RELATIONAL_LT_STRING,

    RVM_CODE_RELATIONAL_LE_INT,
    RVM_CODE_RELATIONAL_LE_INT64,
    RVM_CODE_RELATIONAL_LE_DOUBLE,
    RVM_CODE_RELATIONAL_LE_STRING,

    // bitwise
    RVM_CODE_BITWISE_NOT_INT,
    RVM_CODE_BITWISE_NOT_INT64,
    RVM_CODE_BITWISE_AND_INT,
    RVM_CODE_BITWISE_AND_INT64,
    RVM_CODE_BITWISE_OR_INT,
    RVM_CODE_BITWISE_OR_INT64,
    RVM_CODE_BITWISE_XOR_INT,
    RVM_CODE_BITWISE_XOR_INT64,
    RVM_CODE_BITWISE_LSH_INT,
    RVM_CODE_BITWISE_LSH_INT64,
    RVM_CODE_BITWISE_RSH_INT,
    RVM_CODE_BITWISE_RSH_INT64,

    // jump
    RVM_CODE_JUMP,
    RVM_CODE_JUMP_IF_FALSE,
    RVM_CODE_JUMP_IF_TRUE,

    // duplicate
    RVM_CODE_SHALLOW_COPY,
    RVM_CODE_DEEP_COPY_CLASS_OB,
    RVM_CODE_DEEP_COPY_ARRAY,
    RVM_CODE_POP,
    RVM_CODE_NOP, // 让栈空间增加1，不做额外操作

    // func
    RVM_CODE_PUSH_FUNC,
    RVM_CODE_PUSH_METHOD,
    RVM_CODE_INVOKE_FUNC_NATIVE,
    RVM_CODE_INVOKE_FUNC,
    RVM_CODE_INVOKE_CLOSURE,
    RVM_CODE_INVOKE_METHOD,
    RVM_CODE_RETURN,
    RVM_CODE_FUNCTION_FINISH,

    // closure
    RVM_CODE_NEW_CLOSURE,

    // defer
    RVM_CODE_PUSH_DEFER,
    RVM_CODE_POP_DEFER,


    // coroutine
    RVM_CODE_LAUNCH,
    RVM_CODE_LAUNCH_CLOSURE,
    RVM_CODE_LAUNCH_METHOD,
    RVM_CODE_RESUME,
    RVM_CODE_YIELD,

    // exit
    RVM_CODE_EXIT,


    // 不对应实际的字节码, 不能在生成代码的时候使用
    RVM_CODES_NUM, // 用来标记RVM CODE 的数量
} RVM_Opcode;


typedef enum {
    IDENTIFIER_TYPE_UNKNOW = 0,
    IDENTIFIER_TYPE_VARIABLE,
    IDENTIFIER_TYPE_FUNCTION,
} IdentifierType;


/*
 * RVM_CallInfo 是用来记录函数调用栈的
 * 当调用一个 derive 函数时，则会有一个新的 RVM_CallInfo 被创建
 *
 * code_list: 当前栈运行的字节码数组
 * code_size: 字节码数组大小
 * pc:        PC
 *            保存协程最后被调度时执行的指令, 在协程被重新调度时, 应该+1, 继续执行
 *
 */
struct RVM_CallInfo {
    RVM_ClassObject* caller_object;
    RVM_Function*    caller_function;
    RVM_Closure*     caller_closure;
    unsigned int     caller_stack_base;
    bool             caller_is_defer;

    RVM_ClassObject* callee_object;
    RVM_Function*    callee_function;
    RVM_Closure*     callee_closure;
    unsigned int     callee_argument_size; // 函数调用的参数数量，可变参数

    RVM_Closure*     curr_closure;

    RVM_Byte*        code_list;
    unsigned int     code_size;
    unsigned int     pc; // 在运行字节码的时候，会实时运行

    unsigned int     caller_resume_pc;
    unsigned int     coroutine_resume_pc;
    // 在完成一个函数调用之后，或者完成一个协程调度之后
    // 要进行字节码的恢复，恢复到 resume_pc
    // 只有在函数调用之前、协程调度之前才会被设置

    RVM_CallInfo* prev;
    RVM_CallInfo* next;
};

struct CallInfo {
    unsigned int pc;
    std::string  file;
    std::string  func;
    unsigned int line;
};


typedef enum ErrorEnum {
    RING_INTERNAL_ERROR,

    RING_NIL_ERROR,
    RING_RANGE_ERROR,

    RING_INVALID_OPCODE_ERROR,

    RING_COROUTINE_SCHED_ERROR, // 协程调度错误


    // out of memory
    // stack over flow

} ErrorEnum;

class RuntimeException : public std::exception {

    Ring_VirtualMachine* rvm;
    ErrorEnum            error_num;
    std::string          message;

public:
    RuntimeException(Ring_VirtualMachine* rvm,
                     ErrorEnum            error_num,
                     std::string          message);
    const char* what() const noexcept override;
};

void throw_runtime_err(Ring_VirtualMachine* rvm,
                       ErrorEnum            error_num,
                       const char*          fmt, ...)
    __attribute__((format(printf, 3, 4)));


#define THROW_NIL_FMT "invalid memory address or nil pointer dereference"
#define THROW_NIL_FMT_DETAIL "invalid memory address or nil pointer dereference: %s"
#define THROW_RANGE_FMT "index out of range [%u] with length %u"
#define THROW_INVAID_OPCODE "invalid vm opcode: %d, pc: %d"
#define THROW_COROUTINE_NIL "coroutine is nil, coroutinue id: %llu"
#define THROW_COROUTINE_DEAD "coroutine is dead, coroutinue id: %llu"
#define THROW_COROUTINE_RESUME_SELF "coroutine resume self, coroutinue id: %llu"

/*
 * 报错模板：
 * runtime error: NilError: invalid memory address or nil pointer dereference
 * runtime error: RangeError: index out of range [3] with length 2
 */
#define assert_throw_nil(check) \
    ((check) ? throw_runtime_err((rvm), RING_NIL_ERROR, THROW_NIL_FMT) : (void)0)
#define assert_throw_nil_array(check) \
    ((check) ? throw_runtime_err((rvm), RING_NIL_ERROR, THROW_NIL_FMT_DETAIL, "nil array") : (void)0)
#define assert_throw_nil_closure(check) \
    ((check) ? throw_runtime_err((rvm), RING_NIL_ERROR, THROW_NIL_FMT_DETAIL, "nil closure") : (void)0)

#define assert_throw_range(index, length) \
    (((index) >= (length)) ? throw_runtime_err((rvm), RING_RANGE_ERROR, THROW_RANGE_FMT, (index), (length)) : (void)0)

#define throw_invalid_opcode(opcode, pc) \
    throw_runtime_err((rvm), RING_INVALID_OPCODE_ERROR, THROW_INVAID_OPCODE, (opcode), (pc))

#define assert_throw_nil_coroutine(co_id, coroutine) \
    (((coroutine) == nullptr) ? throw_runtime_err((rvm), RING_COROUTINE_SCHED_ERROR, THROW_COROUTINE_NIL, (co_id)) : (void)0)
#define assert_throw_dead_coroutine(co_id, coroutine) \
    ((((coroutine)->status) == CO_STAT_DEAD) ? throw_runtime_err((rvm), RING_COROUTINE_SCHED_ERROR, THROW_COROUTINE_DEAD, (co_id)) : (void)0)
#define assert_throw_coroutine_resume_self(check, co_id) \
    ((check) ? throw_runtime_err((rvm), RING_COROUTINE_SCHED_ERROR, THROW_COROUTINE_RESUME_SELF, (co_id)) : (void)0)

struct RVM_DeferItem {

    RVM_Closure*   closure;
    unsigned int   argument_size;
    RVM_Value*     argument_list;

    RVM_DeferItem* next;
};


/*
 * Ring_String 是专门给源信息使用的
 * 不是 虚拟机使用的
 * 虚拟机使用 RVM_String
 * TODO: 后续下线
 */
struct Ring_String {
    char*        data;
    unsigned int length;
    unsigned int capacity;
};

typedef enum {
    STATEMENT_EXEC_RESULT_TYPE_UNKNOW = 0,
    STATEMENT_EXEC_RESULT_TYPE_NORMAL,
    STATEMENT_EXEC_RESULT_TYPE_EXPRESSION,
    STATEMENT_EXEC_RESULT_TYPE_RETURN,
    STATEMENT_EXEC_RESULT_TYPE_BREAK,
    STATEMENT_EXEC_RESULT_TYPE_CONTINUE,
} StatementExecResultType;

typedef enum {
    STATEMENT_TYPE_UNKNOW = 0,
    STATEMENT_TYPE_EXPRESSION,
    STATEMENT_TYPE_IF,
    STATEMENT_TYPE_FOR,
    STATEMENT_TYPE_DOFOR,
    STATEMENT_TYPE_BREAK,
    STATEMENT_TYPE_CONTINUE,
    STATEMENT_TYPE_RETURN,
    STATEMENT_TYPE_VAR_DECL,
    STATEMENT_TYPE_TAG_DEFINITION,
    STATEMENT_TYPE_JUMP_TAG,
    STATEMENT_TYPE_DEFER,
} StatementType;

typedef enum {
    EXPRESSION_TYPE_UNKNOW = 0,
    EXPRESSION_TYPE_LITERAL_BOOL,
    EXPRESSION_TYPE_LITERAL_INT,
    EXPRESSION_TYPE_LITERAL_INT64,
    EXPRESSION_TYPE_LITERAL_DOUBLE,
    EXPRESSION_TYPE_LITERAL_STRING,
    EXPRESSION_TYPE_VARIABLE,
    EXPRESSION_TYPE_IDENTIFIER,
    EXPRESSION_TYPE_FUNCTION_CALL,
    EXPRESSION_TYPE_MEMBER_CALL,
    EXPRESSION_TYPE_ASSIGN,

    EXPRESSION_TYPE_TERNARY, // 三目运算

    EXPRESSION_TYPE_ARITHMETIC_ADD,
    EXPRESSION_TYPE_ARITHMETIC_SUB,
    EXPRESSION_TYPE_ARITHMETIC_MUL,
    EXPRESSION_TYPE_ARITHMETIC_DIV,
    EXPRESSION_TYPE_ARITHMETIC_MOD,
    EXPRESSION_TYPE_LOGICAL_AND,
    EXPRESSION_TYPE_LOGICAL_OR,
    EXPRESSION_TYPE_RELATIONAL_EQ,
    EXPRESSION_TYPE_RELATIONAL_NE,
    EXPRESSION_TYPE_RELATIONAL_GT,
    EXPRESSION_TYPE_RELATIONAL_GE,
    EXPRESSION_TYPE_RELATIONAL_LT,
    EXPRESSION_TYPE_RELATIONAL_LE,

    EXPRESSION_TYPE_BITWISE_UNITARY_NOT, // 一元操作符
    EXPRESSION_TYPE_BITWISE_AND,         // 二元 位与
    EXPRESSION_TYPE_BITWISE_OR,          // 二元 位或
    EXPRESSION_TYPE_BITWISE_XOR,         // 二元 位异或
    EXPRESSION_TYPE_BITWISE_LSH,         // 二元 位左移
    EXPRESSION_TYPE_BITWISE_RSH,         // 二元 位右移

    EXPRESSION_TYPE_ARITHMETIC_UNITARY_MINUS, // 一元操作符 负号
    EXPRESSION_TYPE_LOGICAL_UNITARY_NOT,      // 一元操作符 逻辑 非 not
    EXPRESSION_TYPE_UNITARY_INCREASE,         // 一元操作符 i++
    EXPRESSION_TYPE_UNITARY_DECREASE,         // 一元操作符 i--

    EXPRESSION_TYPE_NEW_ARRAY,
    EXPRESSION_TYPE_ARRAY_LITERAL,
    EXPRESSION_TYPE_CLASS_OBJECT_LITERAL,

    EXPRESSION_TYPE_ARRAY_INDEX,
    EXPRESSION_TYPE_SLICE,
    EXPRESSION_TYPE_MEMBER,
    EXPRESSION_TYPE_ELEMENT_ACCESS,

    EXPRESSION_TYPE_CAST,

    EXPRESSION_TYPE_LAUNCH,
    EXPRESSION_TYPE_ANONYMOUS_FUNC,
    EXPRESSION_TYPE_IIFE,

} ExpressionType;

#define EXPR_IS_BASIC_LITERAL(type) \
    ((type) >= EXPRESSION_TYPE_LITERAL_BOOL && (type) <= EXPRESSION_TYPE_LITERAL_STRING)


typedef enum {
    FUNCTION_TYPE_UNKNOW = 0,
    FUNCTION_TYPE_NATIVE,
    FUNCTION_TYPE_DERIVE,
} FunctionType;


typedef enum {
    ASSIGN_EXPRESSION_TYPE_UNKNOW = 0,
    ASSIGN_EXPRESSION_TYPE_ASSIGN,       // =
    ASSIGN_EXPRESSION_TYPE_MULTI_ASSIGN, // a, b = 1, 2
    ASSIGN_EXPRESSION_TYPE_ADD_ASSIGN,   // +=
    ASSIGN_EXPRESSION_TYPE_SUB_ASSIGN,   // -=
    ASSIGN_EXPRESSION_TYPE_MUL_ASSIGN,   // *=
    ASSIGN_EXPRESSION_TYPE_DIV_ASSIGN,   // /=
    ASSIGN_EXPRESSION_TYPE_MOD_ASSIGN,   // %=
} AssignExpressionType;


typedef void (*BuildinFuncFix)(Expression*             expression,
                               FunctionCallExpression* function_call_expression,
                               Block*                  block,
                               Function*               func,
                               Ring_Buildin_Func*      build_func);

typedef void (*BuildinFuncGenerate)(Package_Executer*       executer,
                                    FunctionCallExpression* function_call_expression,
                                    RVM_OpcodeBuffer*       opcode_buffer);


struct Ring_Buildin_Func {
    const char*                 package_posit; // 包名
    const char*                 identifier;

    int                         param_size;
    std::vector<TypeSpecifier*> param_types;

    int                         return_size;
    std::vector<TypeSpecifier*> return_types;

    BuildinFuncFix              buildin_func_fix;
    BuildinFuncGenerate         buildin_func_generate;
};

struct EnumDeclaration {
    std::string          source_file;
    unsigned int         start_line_number; // 源码的开始行
    unsigned int         end_line_number;   // 源码的结束行

    unsigned int         enum_index; // UPDATED_BY_FIX_AST
    TypeSpecifier*       type_specifier;
    char*                identifier;

    unsigned int         enum_item_size;
    EnumItemDeclaration* enum_item_list;
};

struct EnumItemDeclaration {
    unsigned int         line_number;

    unsigned int         index_of_enum; // UPDATED_BY_FIX_AST
    char*                identifier;
    Expression*          value_expr; // 可能是一个表达式, 也可能是一个常量, 编译期间会进行求值

    EnumItemDeclaration* next;
};


struct ClassDefinition {
    PackageUnit*     package_unit; // 所属的 package_unit

    std::string      source_file;
    unsigned int     start_line_number; // 源码的开始行
    unsigned int     end_line_number;   // 源码的结束行

    unsigned int     class_index;
    char*            identifier;


    unsigned int     field_size;
    FieldMember*     field_list;

    unsigned int     method_size;
    MethodMember*    method_list;

    ClassDefinition* next;
};

typedef enum {
    ATTRIBUTE_NONE = 0,
    ACCESS_PUBLIC  = 0x01,
    ACCESS_PRIVATE = 0x01 << 1,
    ACCESS_DELETE  = 0x01 << 2,

    CONSTRUCTOR    = 0x01 << 4,
    DESTRUCTOR     = 0x01 << 5,
} AttributeType;

typedef unsigned int Attribute;

struct AttributeInfo {
    char*          name;
    AttributeInfo* next;
};

typedef enum {
    MEMBER_UNKNOW = 0,
    MEMBER_FIELD,
    MEMBER_METHOD,
} ClassMemberType;

// ClassMemberDeclaration 只用来 在 create_ast的时候, 收集 field method的列表
// create_ast 之后边不再使用
struct ClassMemberDeclaration {
    unsigned int    line_number;

    ClassMemberType type;
    union {
        FieldMember*  field;
        MethodMember* method;
    } u;

    ClassMemberDeclaration* next;
};

// 类成员变量
struct FieldMember {
    unsigned int   line_number;

    unsigned int   index_of_class; // UPDATED_BY_FIX_AST
    TypeSpecifier* type_specifier;
    char*          identifier;

    FieldMember*   next;
};


struct Statement {
    unsigned int  line_number;

    Block*        block; // 所属的 block
    StatementType type;
    union {
        Expression*             expression;
        IfStatement*            if_statement;
        ForStatement*           for_statement;
        DoForStatement*         dofor_statement;
        BreakStatement*         break_statement;
        ContinueStatement*      continue_statement;
        ReturnStatement*        return_statement;
        VarDecl*                var_decl_statement;
        TagDefinitionStatement* tag_definition_statement;
        JumpTagStatement*       jump_tag_statement;
        DeferStatement*         defer_statement;
    } u;
    Statement* next;
};

struct StatementExecResult {
    StatementExecResultType type;
    unsigned int            return_value_list_size;
    Ring_BasicValue**       return_value_list;
};

struct Expression {
    unsigned int line_number;


    /*
     * 一个表达式肯定是有返回值的(除了assign 表达式)(所以 assign是不是应该是个语句, 而不是表达式)
     * 需要通过返回值与后续的操作进行比对, 数据的类型要保持一致, string不能赋值给int变量
     * 对于 function-call/method-call 这种的, 返回值可能有多个, 所以convert_type_size > 1
     *
     * convert_type 统一在 fix_ast中进行更新.
     * fix_ast 中不对 TypeSpecifier进行内存分配, 支持保存指针
     *
     * 但是这里可能很大部分 convert_type_size 都为1, 如果选择
     */
    unsigned int    convert_type_size;
    TypeSpecifier** convert_type;

    ExpressionType  type;
    union {
        bool                          bool_literal;
        int                           int_literal;   // 注意这里是 unsigned
        long long                     int64_literal; // 注意这里是 unsigned
        double                        double_literal;
        const char*                   string_literal;
        IdentifierExpression*         identifier_expression;
        FunctionCallExpression*       function_call_expression;
        MemberCallExpression*         member_call_expression;
        AssignExpression*             assign_expression;
        TernaryExpression*            ternary_expression;
        BinaryExpression*             binary_expression;
        Expression*                   unitary_expression;
        ArrayIndexExpression*         array_index_expression;
        SliceExpression*              slice_expression;
        NewArrayExpression*           new_array_expression;
        ArrayLiteralExpression*       array_literal_expression;
        ClassObjectLiteralExpression* class_object_literal_expression;
        CastExpression*               cast_expression;
        MemberExpression*             member_expression;
        FieldInitExpression*          field_init_expression;
        LaunchExpression*             launch_expression;
        AnonymousFuncExpression*      anonymous_func_expression;
        ImmediateInvokFuncExpression* iife;
    } u;

    Expression* next;
};


// 清除 expression 所代表的 convert_type_size 和 convert_type
#define EXPRESSION_CLEAR_CONVERT_TYPE(expression) \
    (expression)->convert_type_size = 0;          \
    (expression)->convert_type      = nullptr;

// 添加 expression 所代表的 convert_type_size 和 convert_type
// 其实 只有 function-call/method-call 表达式: convert_type_size > 1
// 其余的都是 convert_type_size = 1
#define EXPRESSION_ADD_CONVERT_TYPE(expression, type_specifier)                             \
    (expression)->convert_type_size = (expression)->convert_type_size + 1;                  \
    (expression)->convert_type =                                                            \
        (TypeSpecifier**)realloc((expression)->convert_type,                                \
                                 (expression)->convert_type_size * sizeof(TypeSpecifier*)); \
    (expression)->convert_type[(expression)->convert_type_size - 1] = (type_specifier);


typedef enum {
    IDENTIFIER_EXPRESSION_TYPE_UNKNOW,
    IDENTIFIER_EXPRESSION_TYPE_VARIABLE,
    IDENTIFIER_EXPRESSION_TYPE_FUNC,
    IDENTIFIER_EXPRESSION_TYPE_ENUM_ITEM,
} IdentifierExpressionType;

struct IdentifierExpression {
    unsigned int line_number;
    unsigned int row_number;

    char*        path_segment;
    // path_segment 代表 a::b
    // a 可以代表一个package
    // b 可以表示一个enum
    // 后续可以是一个空字符串

    IdentifierExpressionType type;
    char*                    identifier;
    union {
        Variable*            variable;
        Function*            function;
        EnumItemDeclaration* enum_item;
    } u;
};


typedef enum {
    UNKNOW_COPY,
    SHALLOW_COPY,
    DEEP_COPY,
} RVM_Value_Copy_Type;


struct ArrayIndexExpression {
    unsigned int         line_number;

    Expression*          array_expression;
    DimensionExpression* index_expression;
};

struct MemberExpression {
    unsigned int line_number;

    Expression*  object_expression;
    char*        field_member_identifier;
    FieldMember* field_member; // UPDATED_BY_FIX_AST
};


struct NewArrayExpression {
    unsigned int         line_number;

    TypeSpecifier*       type_specifier;
    DimensionExpression* dimension_expression;
};

struct ArrayLiteralExpression {
    unsigned int         line_number;

    TypeSpecifier*       type_specifier;
    DimensionExpression* dimension_expression;
    Expression*          expression_list;
};

struct ClassObjectLiteralExpression {
    unsigned int         line_number;

    TypeSpecifier*       type_specifier;
    FieldInitExpression* field_init_expression_list;
};

struct CastExpression {
    unsigned int   line_number;

    TypeSpecifier* type_specifier;
    Expression*    operand;
};


/*
 * DimensionExpression
 *
 * Dimension是有语法糖的，
 *
 * [,,] 是一个三维数组, 可以用来指示变量的类型
 * [!3] 是一个三维数组, 可以用来指示变量的类型
 *
 * [,,] [!3] 不能用来 new 数组
 *
 *
 * [1,2,3] 只能用这种形式new数组
 *
 * TODO: 要在语义分析的层面做细致的检测
 */
struct DimensionExpression {
    unsigned int            line_number;

    unsigned char           dimension;
    SubDimensionExpression* dimension_list;
};
struct SubDimensionExpression {
    unsigned int            line_number;
    unsigned char           index;
    Expression*             num_expression;
    SubDimensionExpression* next;

    // field: index
    // index = 2 为二维数组

    // field: num_expression
    // when new array, num_expression is array size.
    // when access array, num_expression is index.
};

typedef enum {
    SLICE_OPERAND_TYPE_UNKNOW,
    SLICE_OPERAND_TYPE_ARRAY,
    SLICE_OPERAND_TYPE_STRING,
} SliceOperandType;
struct SliceExpression {
    unsigned int        line_number;

    SliceOperandType    slice_operand_type; // UPDATED_BY_FIX_AST
    Expression*         operand;
    SubSliceExpression* sub_slice;
};
struct SubSliceExpression {
    unsigned int line_number;
    Expression*  start_expr;
    Expression*  end_expr;
};

typedef enum {
    RANGE_EXPRESSION_TYPE_UNKNOW = 0,
    RANGE_EXPRESSION_TYPE_STEP,
    RANGE_EXPRESSION_TYPE_LINEAR,
} SubRangeExpressionType;
struct RangeExpression {
    unsigned int line_number;

    // 与 Expression 中概念相同
    unsigned int           convert_type_size; // UPDATED_BY_FIX_AST
    TypeSpecifier**        convert_type;      // UPDATED_BY_FIX_AST

    SubRangeExpressionType type;
    union {
        SubStepRangeExpression*   sub_step_range_expr;
        SubLinearRangeExpression* sub_linear_range_expr;
    } u;
};
// 算术步长范围
struct SubStepRangeExpression {
    unsigned int line_number;

    Expression*  start_expr;
    Expression*  end_expr;
    Expression*  step_expr;

    // true  end_expr为闭区间
    // false end_expr开区间
    bool is_inclusive;

    // 需要生成什么类型的step
    // int64 or double
    // 需要 通过 start_expr/end_expr/step_expr 综合判断
    Ring_BasicType value_type; // UPDATED_BY_FIX_AST
};
// 线性范围：集合相关
struct SubLinearRangeExpression {
    unsigned int line_number;

    // 目前只支持数组
    // 后续可以支持map
    Expression* collection_expr; // 数组，字符串，map
};

// TODO:
// 专门用来给 array string 的 index使用
// 后续需要固定为 64位、32位，参考rust实现
// 后续改成 unsigned long long
using usize = unsigned long long;

//
using RVM_RangeIterator_RangeValue = std::variant<
    int,
    long long,
    double,
    std::tuple<usize, bool>,
    std::tuple<usize, int>,
    std::tuple<usize, long long>,
    std::tuple<usize, double>,
    std::tuple<usize, RVM_String*>,
    std::tuple<usize, RVM_ClassObject*>,
    std::tuple<usize, RVM_Array*>,
    std::tuple<usize, RVM_Closure*>>;

class RVM_RangeIterator {
public:
    virtual bool                         has_next() = 0;
    virtual RVM_RangeIterator_RangeValue get_next() = 0;
};


template <typename T>
concept AllowedTypes = std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, double>;
template <AllowedTypes T>
class RVM_StepRangeIterator : public RVM_RangeIterator {
    T    current;
    T    end;
    T    step;
    bool is_inclusive; // end 是否为闭区间

public:
    RVM_StepRangeIterator(T start, T end, T step, bool is_inclusive) :
        current(start),
        end(end),
        step(step),
        is_inclusive(is_inclusive) {
    }
    virtual bool has_next() {
        if (step > 0)
            return is_inclusive ? (current <= end) : (current < end);
        else
            return is_inclusive ? (current >= end) : (current > end);
    }
    virtual RVM_RangeIterator_RangeValue get_next() {
        auto val = current;
        current += step;
        return val;
    }
};

class RVM_LinearRangeIterator : public RVM_RangeIterator {
private:
    RVM_Array* array_value;
    long long  index; // 数组的索引

public:
    RVM_LinearRangeIterator(RVM_Array* array_value) :
        array_value(array_value),
        index(0) {
        };
    virtual bool has_next() {
        return index < array_value->length;
    };
    virtual RVM_RangeIterator_RangeValue get_next() {
        RVM_RangeIterator_RangeValue result;

        switch (array_value->type) {
        case RVM_ARRAY_BOOL:
            result.emplace<std::tuple<usize, bool>>(index, array_value->u.bool_array[index]);
            break;
        case RVM_ARRAY_INT:
            result.emplace<std::tuple<usize, int>>(index, array_value->u.int_array[index]);
            break;
        case RVM_ARRAY_INT64:
            result.emplace<std::tuple<usize, long long>>(index, array_value->u.int64_array[index]);
            break;
        case RVM_ARRAY_DOUBLE:
            result.emplace<std::tuple<usize, double>>(index, array_value->u.double_array[index]);
            break;
        case RVM_ARRAY_STRING:
            result.emplace<std::tuple<usize, RVM_String*>>(index, array_value->u.string_array[index]);
            break;
        case RVM_ARRAY_CLASS_OBJECT:
            result.emplace<std::tuple<usize, RVM_ClassObject*>>(index, array_value->u.class_ob_array[index]);
            break;
        case RVM_ARRAY_A:
            result.emplace<std::tuple<usize, RVM_Array*>>(index, array_value->u.a_array[index]);
            break;
        case RVM_ARRAY_CLOSURE:
            result.emplace<std::tuple<usize, RVM_Closure*>>(index, array_value->u.closure_array[index]);
            break;
        default: break;
        }

        index++;

        return result;
    };
};


typedef enum {
    FUNCTION_CALL_TYPE_UNKNOW,
    FUNCTION_CALL_TYPE_FUNC,
    FUNCTION_CALL_TYPE_CLOSURE,
} FunctionCallType;


struct FunctionCallExpression {
    unsigned int line_number;

    char*        package_posit;
    char*        func_identifier;
    Expression*  func_expr;
    /*
     * TODO:
     * 后续应该 删除 package_posit func_identifier
     * 只保留 func_expr
     *
     * 因为 expression再为 IdentifierExpression 的时候，是包含了 package_posit func_identifier 的
     * 但是需要改造的代码较多，后续可以修改
     *
     * 目前：
     * func_expr != EXPRESSION_TYPE_IDENTIFIER :
     *   取 func_expr
     * func_expr != EXPRESSION_TYPE_IDENTIFIER :`
     *   取 func_identifier
     */

    FunctionCallType type;
    union {
        struct {
            Function* function; // UPDATED_BY_FIX_AST
        } fc;                   // function-call
        struct {
            VarDecl* closure_decl; // UPDATED_BY_FIX_AST
        } cc;                      // closure-call

    } u;

    unsigned int  argument_list_size;
    ArgumentList* argument_list;
};

typedef enum {
    MEMBER_CALL_TYPE_UNKNOW,
    MEMBER_CALL_TYPE_METHOD,
    MEMBER_CALL_TYPE_FIELD,
} MemberCallType;

struct MemberCallExpression {
    unsigned int   line_number;

    Expression*    object_expression;
    char*          member_identifier;

    MemberCallType type;
    union {
        struct {
            MethodMember* method_member; // UPDATED_BY_FIX_AST
        } mc;                            // method-call, a.method()
        struct {
            FieldMember* field_member; // UPDATED_BY_FIX_AST
        } fc;                          // field 的类型为匿名函数数组, a.field()
    } u;

    unsigned int  argument_list_size;
    ArgumentList* argument_list;
};

struct Identifier {
    unsigned int   line_number;

    IdentifierType type;
    char*          name;
    unsigned int   array_index;  // 供数组使用，还要考虑一下负值索引的问题
    Function*      parent_scope; // 作用域

    Identifier*    next;
};

struct AssignExpression {
    unsigned int         line_number;

    AssignExpressionType type;
    Expression*          left;
    Expression*          operand;
};

struct FieldInitExpression {
    unsigned int         line_number;

    char*                field_identifier;
    FieldMember*         field_member; // UPDATED_BY_FIX_AST
    Expression*          init_expression;
    FieldInitExpression* next;
};

typedef enum {
    LAUNCH_EXPRESSION_TYPE_UNKNOW = 0,
    LAUNCH_EXPRESSION_TYPE_FUNCTION_CALL,
    LAUNCH_EXPRESSION_TYPE_MEMBER_CALL,
    LAUNCH_EXPRESSION_TYPE_IIFE,
    // LAUNCH_EXPRESSION_TYPE_BLOCK,
} LaunchExpressionType;

struct LaunchExpression {
    unsigned int         line_number;

    LaunchExpressionType type;

    union {
        FunctionCallExpression*       function_call_expression;
        MemberCallExpression*         member_call_expression;
        ImmediateInvokFuncExpression* iife;
    } u;
};

struct AnonymousFuncExpression {
    unsigned int   line_number;

    AnonymousFunc* anonymous_func;
};

struct ImmediateInvokFuncExpression {
    unsigned int   line_number;

    AnonymousFunc* anonymous_func;
    unsigned int   argument_list_size;
    ArgumentList*  argument_list;
};

struct BinaryExpression {
    unsigned int line_number;

    Expression*  left_expression;
    Expression*  right_expression;
};

// 三元运算符
struct TernaryExpression {
    unsigned int line_number;

    Expression*  condition_expression;
    Expression*  true_expression;
    Expression*  false_expression;
};

struct ArgumentList {
    unsigned int  line_number;

    Expression*   expression;
    ArgumentList* next;
};

struct Parameter {
    unsigned int   line_number;

    TypeSpecifier* type_specifier;
    bool           is_variadic; // variadic parameter function can be called with any number of trailing arguments.
    char*          identifier;
    Parameter*     next;
};

struct VarDecl {
    unsigned int   line_number;

    Block*         blong_block;

    TypeSpecifier* type_specifier;
    char*          identifier;
    int            is_const;
    int            is_local;
    int            variable_index; // 全局变量/局部变量的索引
    VarDecl*       next;           // TODO: 这里设计的优点混乱了
};

struct Variable {
    VarDecl*       decl;
    bool           is_free_value;
    FreeValueDesc* free_value_desc;

    Variable*      next;
};

typedef enum {
    BLOCK_TYPE_UNKNOW,
    BLOCK_TYPE_IF,
    BLOCK_TYPE_FOR,
    BLOCK_TYPE_DOFOR,
    BLOCK_TYPE_FUNCTION,
} BlockType;

typedef struct BlockLabels {
    unsigned int break_label;
    unsigned int continue_label;
} BlockLabels;


/*
block 中的 declaration 是不是考虑应该放在外边

为什么：局部变量的初始化时机不太正确。因为局部量只有在函数调用的时候才进行初始化
*/
struct Block {
    unsigned int   start_line_number;
    unsigned int   end_line_number;

    BlockType      type;

    unsigned int   var_decl_list_size;
    VarDecl*       var_decl_list; // 代码块中定义的局部变量

    unsigned int   free_value_size;
    FreeValueDesc* free_value_list; // 自由变量, 直接或者间接

    unsigned int   visable_var_size;
    Variable*      visable_var_list; // 可见的变量, 搜索过的，用于提高fix_ast 的速度

    unsigned int   statement_list_size;
    Statement*     statement_list;

    Block*         parent_block;

    BlockLabels    block_labels;

    PackageUnit*   package_unit; // 所属的package_unit
};


#define FUNCTION_TUPLE_HEADER                                                    \
    std::string         source_file;       /*ring source file*/                  \
    unsigned int        start_line_number; /*start line no in ring source file*/ \
    unsigned int        end_line_number;   /*end   line no in ring source file*/ \
    RingFileStat*       ring_file_stat;    /*ring source file stat*/             \
                                                                                 \
    unsigned int        parameter_list_size;                                     \
    Parameter*          parameter_list;                                          \
    unsigned int        return_list_size;                                        \
    FunctionReturnList* return_list;                                             \
    Block*              block;                                                   \
    FunctionTuple*      next;


struct FunctionTuple {
    FUNCTION_TUPLE_HEADER;
};

struct AnonymousFunc {
    FUNCTION_TUPLE_HEADER;
};

struct Function {
    FUNCTION_TUPLE_HEADER;

    Package*     package;
    char*        identifier;
    unsigned int func_index; // 所在 package 的 index
    FunctionType type;
};

struct MethodMember {
    FUNCTION_TUPLE_HEADER;

    Package*     package;
    char*        identifier;
    unsigned int index_of_class; // 所在类的index UPDATED_BY_FIX_AST
};

struct FreeValueDesc {
    char* identifier;
    bool  is_curr_local; // 是本层函数定义的局部变量
    union {
        unsigned int curr_local_index; // 如果是直接外层函数的局部变量，外层函数的局部变量索引
        unsigned int out_free_value_index;
    } u;

    unsigned       free_value_index; // UPDATED_BY_FIX_AST
    FreeValueDesc* next;
};

struct FunctionReturnList {
    unsigned int        line_number;

    TypeSpecifier*      type_specifier;
    FunctionReturnList* next;
};

struct IfStatement {
    unsigned int     line_number;

    Expression*      condition_expression;

    Block*           if_block;
    ElseIfStatement* elseif_list;
    Block*           else_block;
};

struct ElseIfStatement {
    unsigned int     line_number;

    Expression*      condition_expression;

    Block*           elseif_block;

    ElseIfStatement* next;
};


typedef enum {
    FOR_STATEMENT_TYPE_UNKNOW = 0,
    FOR_STATEMENT_TYPE_TERNARY,
    FOR_STATEMENT_TYPE_RANGE,
} ForStatementType;
/*
 * ForStatement 有两种形式
 *
 * 1. c语言传统 三元For语句
 *    for(index = 0; index < 10; index++) {
 *
 *    }
 *
 * `index = 0`   : ternary_statement.init_expression
 * `index < 10`  : ternary_statement.condition_expression
 * `index++`     : ternary_statement.post_expression
 *
 * 2. 较为现代的 rang语句
 *    for(index, value = range(int_array)) {
 *
 *    }
 *
 * `index, value` : range_statement.assign_identifier_list
 * `int_array`    : range_statement.array_identifier
 */

struct ForStatement {
    unsigned int     line_number;

    ForStatementType type;

    union {
        ForTernaryStatement* ternary_statement;
        ForRangeStatement*   range_statement;
    } u;

    Block* block;
};

struct ForTernaryStatement {
    Expression* init_expression;
    Expression* condition_expression;
    Expression* post_expression;
};

struct ForRangeStatement {
    Expression*      left;
    RangeExpression* range_expr;
};

struct DoForStatement {
    unsigned int line_number;

    Expression*  init_expression;
    Block*       block;
    Expression*  condition_expression;
    Expression*  post_expression;
};


struct BreakStatement {
    unsigned int line_number;

    unsigned int break_loop_num;
    // 语法:  break; break 1; break 2;

    Block* jump_to_block; // UPDATED_BY_FIX_AST: break 语句跳转位置 所在的block
};

struct TagDefinitionStatement {
    unsigned int line_number;

    char*        identifier;
};

struct JumpTagStatement {
    unsigned int line_number;

    char*        identifier;
};

struct DeferStatement {
    unsigned int                  line_number;

    ImmediateInvokFuncExpression* iife;
};

struct ContinueStatement {
    unsigned int line_number;

    Block*       jump_to_block; // UPDATED_BY_FIX_AST: continue 语句跳转位置 所在的block
};

struct ReturnStatement {
    unsigned int line_number;

    unsigned int return_list_size;
    Expression*  return_list;
};


struct StdPackageNativeFunction {
    char*               identifier;
    RVM_NativeFuncProc* func_proc;
    int                 arg_count;
    int                 return_list_count;
};

struct StdPackageInfo {
    char*                                 package_name;
    char*                                 path;

    std::vector<StdPackageNativeFunction> native_function_list;
};


typedef enum {
    RVM_DEBUG_MODE_UNKNOW,

    RVM_DEBUG_MODE_STEPINTO,
    RVM_DEBUG_MODE_STEPOVER,
    RVM_DEBUG_MODE_STEPOUT,

} RVM_DebugMode;


#define RING_CMD_T_RUN "run"
#define RING_CMD_T_BUILD "build"
#define RING_CMD_T_DUMP "dump"
#define RING_CMD_T_RDB "rdb"
#define RING_CMD_T_MAN "man"
#define RING_CMD_T_VERSION "version"
#define RING_CMD_T_HELP "help"

#define RING_RDB_INTERPRETER_DAP "dap" // DAP 交互协议

#define FUNC_GLOBAL_INIT "__global_init"
#define FUNC_MAIN "main"
#define PACKAGE_MAIN "main"

enum RING_COMMAND_TYPE {
    RING_COMMAND_UNKNOW,

    RING_COMMAND_RUN,
    RING_COMMAND_BUILD,
    RING_COMMAND_DUMP,
    RING_COMMAND_RDB,

    RING_COMMAND_MAN,
    RING_COMMAND_VERSION,
    RING_COMMAND_HELP,
};

struct Ring_Command_Arg {
    RING_COMMAND_TYPE        cmd;

    std::string              input_file_name; // run/dump/rdb
    std::string              keyword;         // man
    unsigned int             optimize_level;
    std::string              rdb_interpreter; // rdb 交互协议，默认为 命令行模式
    std::vector<std::string> shell_args;
};

// rdb cmd token
#define VM_DB_CMD_T_HELP "help"
#define VM_DB_CMD_T_CLEAR "clear"
#define VM_DB_CMD_T_QUIT "quit"

#define VM_DB_CMD_T_STACK "stack"

enum VM_DB_COMMAND_TYPE {
    VM_DB_COMMAND_UNKNOW,

    VM_DB_COMMAND_HELP,
    VM_DB_COMMAND_CLEAR,
    VM_DB_COMMAND_QUIT,

    VM_DB_COMMAND_STACK,
};
struct VM_DB_Arg {
    VM_DB_COMMAND_TYPE cmd;

    std::string        argument;
};


// rdb cmd token
#define RDB_CMD_T_HELP "help"
#define RDB_CMD_T_CLEAR "clear"
#define RDB_CMD_T_RUN "run"
#define RDB_CMD_T_QUIT "quit"

#define RDB_CMD_T_GLOBAL "global"
#define RDB_CMD_T_LOCAL "local"
#define RDB_CMD_T_RUN "run"
#define RDB_CMD_T_CONT "cont"
#define RDB_CMD_T_BT "bt"

#define RDB_CMD_T_BREAK "break"
#define RDB_CMD_T_BREAK_SET "set"
#define RDB_CMD_T_BREAK_UNSET "unset"
#define RDB_CMD_T_BREAK_LIST "list"
#define RDB_CMD_T_BREAK_CLEAR "clear"

#define RDB_CMD_T_STEP "step"
#define RDB_CMD_T_STEP_OVER "over"
#define RDB_CMD_T_STEP_INTO "into"
#define RDB_CMD_T_STEP_OUT "out"


#define RDB_CMD_T_CODE "code"
#define RDB_CMD_T_CODE_LIST "list"


enum RDB_COMMAND_TYPE {
    RDB_COMMAND_UNKNOW,

    RDB_COMMAND_HELP,
    RDB_COMMAND_CLEAR,
    RDB_COMMAND_RUN,
    RDB_COMMAND_QUIT,

    RDB_COMMAND_GLOBAL,
    RDB_COMMAND_LOCAL,
    RDB_COMMAND_CONT,
    RDB_COMMAND_BT,

    RDB_COMMAND_BREAK,

    RDB_COMMAND_STEP,

    RDB_COMMAND_CODE,

};

enum RDB_COMMAND_BREAK_TYPE {
    RDB_COMMAND_BREAK_UNKNOW,
    RDB_COMMAND_BREAK_SET,
    RDB_COMMAND_BREAK_UNSET,
    RDB_COMMAND_BREAK_LIST,
    RDB_COMMAND_BREAK_CLEAR,
};

enum RDB_COMMAND_STEP_TYPE {
    RDB_COMMAND_STEP_UNKNOW,
    RDB_COMMAND_STEP_OVER,
    RDB_COMMAND_STEP_INTO,
    RDB_COMMAND_STEP_OUT,
};

enum RDB_COMMAND_CODE_TYPE {
    RDB_COMMAND_CODE_UNKNOW,
    RDB_COMMAND_CODE_LIST,
};

struct RDB_Command {
    std::string              token;
    std::vector<std::string> rule;
    std::string              description;

    std::vector<RDB_Command> sub_command;
};


struct RDB_Arg {
    RDB_COMMAND_TYPE         cmd;
    RDB_COMMAND_BREAK_TYPE   break_cmd;
    RDB_COMMAND_STEP_TYPE    step_cmd;
    RDB_COMMAND_CODE_TYPE    code_cmd;

    std::string              argument;
    std::vector<std::string> shell_args;
};


#define TRACE_EVENT_SAE "stop-at-entry"
#define TRACE_EVENT_OPCODE "opcode"
#define TRACE_EVENT_LINE "line"
#define TRACE_EVENT_CALL "call"
#define TRACE_EVENT_RETURN "return"
#define TRACE_EVENT_EXIT "exit"


#define ENABLE_TRACE_EVENT_SAE (1 << 0)
#define ENABLE_TRACE_EVENT_OPCODE (1 << 1)
#define ENABLE_TRACE_EVENT_LINE (1 << 2)
#define ENABLE_TRACE_EVENT_CALL (1 << 3)
#define ENABLE_TRACE_EVENT_RETURN (1 << 4)
#define ENABLE_TRACE_EVENT_EXIT (1 << 5)
#define ENABLE_TRACE_EVENT_ALL (ENABLE_TRACE_EVENT_SAE      \
                                | ENABLE_TRACE_EVENT_OPCODE \
                                | ENABLE_TRACE_EVENT_LINE   \
                                | ENABLE_TRACE_EVENT_CALL   \
                                | ENABLE_TRACE_EVENT_RETURN \
                                | ENABLE_TRACE_EVENT_EXIT)

#define DISABLE_TRACE_EVENT_SAE (~ENABLE_TRACE_EVENT_SAE)
#define DISABLE_TRACE_EVENT_OPCODE (~ENABLE_TRACE_EVENT_OPCODE)
#define DISABLE_TRACE_EVENT_LINE (~ENABLE_TRACE_EVENT_LINE)
#define DISABLE_TRACE_EVENT_CALL (~ENABLE_TRACE_EVENT_CALL)
#define DISABLE_TRACE_EVENT_RETURN (~ENABLE_TRACE_EVENT_RETURN)
#define DISABLE_TRACE_EVENT_EXIT (~ENABLE_TRACE_EVENT_EXIT)
#define DISABLE_TRACE_EVENT_ALL (~ENABLE_TRACE_EVENT_ALL)

#define SET_TRACE_EVENT_SAE(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_SAE)
#define SET_TRACE_EVENT_OPCODE(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_OPCODE)
#define SET_TRACE_EVENT_LINE(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_LINE)
#define SET_TRACE_EVENT_CALL(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_CALL)
#define SET_TRACE_EVENT_RETURN(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_RETURN)
#define SET_TRACE_EVENT_EXIT(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_EXIT)
#define SET_TRACE_EVENT_ALL(debug_config) ((debug_config)->enable_trace_event |= ENABLE_TRACE_EVENT_ALL)

#define UNSET_TRACE_EVENT_SAE(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_SAE)
#define UNSET_TRACE_EVENT_OPCODE(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_OPCODE)
#define UNSET_TRACE_EVENT_LINE(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_LINE)
#define UNSET_TRACE_EVENT_CALL(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_CALL)
#define UNSET_TRACE_EVENT_RETURN(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_RETURN)
#define UNSET_TRACE_EVENT_EXIT(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_EXIT)
#define UNSET_TRACE_EVENT_ALL(debug_config) ((debug_config)->enable_trace_event &= DISABLE_TRACE_EVENT_ALL)


#define ISSET_TRACE_EVENT_SAE(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_SAE)
#define ISSET_TRACE_EVENT_OPCODE(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_OPCODE)
#define ISSET_TRACE_EVENT_LINE(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_LINE)
#define ISSET_TRACE_EVENT_CALL(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_CALL)
#define ISSET_TRACE_EVENT_RETURN(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_RETURN)
#define ISSET_TRACE_EVENT_EXIT(debug_config) ((debug_config)->enable_trace_event & ENABLE_TRACE_EVENT_EXIT)
#define ISSET_TRACE_EVENT_ALL(debug_config) ((debug_config)->enable_trace_event == ENABLE_TRACE_EVENT_ALL)


#define DEBUG_CONFIG(frame) ((frame)->rvm->debug_config)
#define DEBUG_IS_DAP(debug_config) (true == str_eq((debug_config)->rdb_interpreter.c_str(), RING_RDB_INTERPRETER_DAP))

struct RVM_DebugConfig {
    bool                  enable;
    TraceDispacth         trace_dispatch;
    unsigned char         enable_trace_event;


    bool                  stop_at_entry;
    bool                  display_globals;
    bool                  display_locals;
    bool                  display_call_stack;

    RDB_COMMAND_STEP_TYPE step_cmd;
    unsigned int          call_func_deep_count;
    unsigned int          step_over_deep_count; // 记录刚才 step-over 操作的位置
    unsigned int          step_out_deep_count;  // 记录刚才 step—out 操作的位置


    // break_points 先简单实现, 只能在 main package 中设置断点
    std::vector<RVM_BreakPoint> break_points;
    // RVM_DebugMode debug_mode;

    std::string rdb_interpreter;
};

struct RVM_BreakPoint {
    // TODO: package_executer
    char*        package;
    char*        file_name;
    char*        func_name;
    unsigned int line_number;
};

struct RVM_Frame {
    Ring_VirtualMachine*                            rvm;
    RVM_CallInfo*                                   call_info;

    unsigned int                                    next_pc;
    RVM_Opcode                                      next_opcode;
    unsigned int                                    source_line_number;

    std::vector<std::pair<std::string, RVM_Value*>> globals;
    std::vector<std::pair<std::string, RVM_Value*>> locals;

    const char*                                     event;
    int                                             exit_code;
};

typedef enum {
    ERROR_CODE_SUCCESS,

    // 命令行参数错误
    ERROR_CODE_COMMAND_ERROR,

    // 编译错误
    ERROR_CODE_COMPILE_ERROR,

    // 语义分析错误
    ERROR_CODE_SEMANTIC_CHECH_ERROR,


    ERROR_CODE_GRAMMAR_ERROR                    = 100000, // 语法错误
    ERROR_CODE_UNKNOW_PACKAGE                   = 100001, // 找不到 package

    ERROR_INVALID_TERNARY_EXPR_CONDITION        = 100002, // 三目条件运算符 不合法的条件表达式
    ERROR_INVALID_TERNARY_EXPR_TRUE_OR_FALSE    = 100003, // 三目条件运算符 不合法true/false条件表达式
    ERROR_TYPEDEF_NAME_NOT_UPPERCASE_LETTER     = 100004, // typedef 定义的类型名称必须大写开头
    ERROR_DEF_PACKAGE_NAME_NOT_LOWERCASE_LETTER = 100005, // def package 定义的 package 名称必须小写开头
    ERROR_USE_PACKAGE_MAIN_IMPORT               = 100006, // use package 不能导入 main
    ERROR_NOT_FOUND_MAIN_FUNCTION               = 100007, // 找不到 main 函数


    ERROR_UNDEFINITE_VARIABLE                   = 200000,
    ERROR_REDEFINITE_GLOBAL_VARIABLE            = 200001, // 重复定义全局变量
    ERROR_REDEFINITE_FUNCTION                   = 200002, // 重复定义函数
    ERROR_OPER_INVALID_USE                      = 200003, // - 符号 不合法使用
    ERROR_DUPLICATE_IMPORT_PACKAGE              = 200004, // 重复 import package
    ERROR_INVALID_VARIABLE_IDENTIFIER           = 200005, // 不合法的变量标识符
    ERROR_TOO_MANY_LOCAL_VARIABLES              = 200006, // 局部变量数量超过限制

    ERROR_ARRAY_ACCESS_DIMENSION_INVALID        = 200009, // 数组访问维度不合法
    ERROR_ARRAY_DIMENSION_INVALID               = 200010, // 定义数组维度不合法
    ERROR_UNCLOSED_STRING_LITERAL               = 200011, // 未关闭的字符串常量
    ERROR_FUNCTION_CALL_IN_MULTIPLE_OPERANDS    = 200012, // 函数调用用到 多项赋值中

    ERROR_ASSIGNMENT_MISMATCH_NUM               = 200020, // 赋值时, 左值和右值的数量不匹配
    ERROR_ASSIGNMENT_MISMATCH_TYPE              = 200021, // 赋值时, 左值和右值的类型不匹配
    ERROR_CONCAT_OPERATOR_MISMATCH_TYPE         = 200022, // string .. 操作数类型不不匹配
    ERROR_MATH_OPERATOR_MISMATCH_TYPE           = 200023, // 数学运算操作数不是 int/int64/double 类型
    ERROR_REDEFINITE_LOCAL_VARIABLE             = 200024, // 重复定义的局部变量
    ERROR_ARRAY_LITERAL_INVALID_ITEM            = 200025, // 数组字面量中，不合法的元素
    ERROR_ARRAY_LITERAL_MISMATCH_TYPE           = 200026, // 不合法的数组常量语句，类型不匹配
    ERROR_SLICE_OPERATOR_MISMATCH_TYPE          = 200027, // 不合法的 slice 操作数类型

    ERROR_REDEFINITE_CLASS                      = 200030, // 重复定义 class
    ERROR_REDEFINITE_MEMBER_IN_CLASS            = 200031, // 重复定义 class field/method
    ERROR_TOO_MANY_FIELDS_IN_CLASS              = 200032, // class 中 field 的数量超过限制
    ERROR_TOO_MANY_METHODS_IN_CLASS             = 200033, // class 中 method 的数量超过限制
    ERROR_MISS_CLASS_DEFINITION                 = 200034, // 缺少 class 定义
    ERROR_INVALID_FIELD_IN_CLASS                = 200035, // field 不合法
    ERROR_ASSIGN_TO_METHOD_OF_CLASS             = 200036, // 不能给 class中 method赋值
    ERROR_INVOKE_FIELD_OF_CLASS_AS_METHOD       = 200037, // 不能将 class field作为匿名函数去 调用
    ERROR_NOT_FOUND_CLASS_MEMBER                = 200038, // 找不到 class field或者method
    ERROR_NOT_FOUND_CLASS_FIELD                 = 200039, // 找不到 class field
    ERROR_NOT_FOUND_CLASS_METHOD                = 200040, // 找不到 class method
    ERROR_DUPLICATE_INIT_CLASS_FIELD            = 200041, // class field 初始化重复
    ERROR_ARRAY_LITERAL_INVALID_FILED_INIT_EXPR = 200042, // 不合法的field初始表达式

    ERROR_OVERFLOWS                             = 200051, // 溢出

    ERROR_UNDEFINITE_FUNCTION                   = 300000,
    ERROR_ARGUMENT_MISMATCH_NUM                 = 300001, // 函数调用参数数量类型不匹配
    ERROR_ARGUMENT_MISMATCH_TYPE                = 300002, // 函数调用参数数量类型不匹配
    ERROR_FUNCTION_INVALID_VARIADIC_PARAMETER   = 300003, // 函数定义中不合法的 可变参数

    ERROR_FUNCTION_MISMATCH_RETURN_NUM          = 300004, // 函数定义的返回值和return中的返回值类型不匹配
    ERROR_FUNCTION_MISMATCH_RETURN_TYPE         = 300005, // 函数定义的返回值和return中的返回值类型不匹配

    ERROR_FUNCTION_MAIN_INVALID                 = 300006, // main函数定义不合法

    ERROR_CANOT_USE_VAR_LIKE_FUNC               = 300007, // 不能使用变量名作为函数名
    ERROR_RANGE_OPERAND_IS_NOT_ARRAY            = 300008, // range 操作数不是数组
    ERROR_RANGE_ASSIGN_MISSMATCH_TYPE           = 300009, // range 赋值时, 左值和右值的类型不匹配

    ERROR_VAR_IS_NOT_ARRAY                      = 300011, // 变量不是数组
    ERROR_INVALID_BREAK_STATEMENT               = 300012, // break 语句不合法
    ERROR_INVALID_CONTINUE_STATEMENT            = 300013, // continue 语句不合法
    ERROR_FUNCTION_MISS_BLOCK                   = 300014, // 函数缺少代码块
    ERROR_NATIVE_FUNCTION_PROVIDE_BLOCK         = 300015, // native 函数禁止提供代码块

    ERROR_NOT_FOUND_ENUM                        = 300020, // 找不到 enum
    ERROR_NOT_SUPPORT_ENUM_TYPE                 = 300021, // 不支持的 enum 类型
    ERROR_NOT_SUPPORT_ENUM_VALUE                = 300022, // 不支持的 enum 值

    // 优化AST错误
    ERROR_CODE_OPTIMIZATION_AST_ERROR,


    // 生成虚拟机代码错误
    ERROR_CODE_GENERATE_OPCODE_ERROR,
    ERROR_CODE_FIX_LABEL,
    ERROR_CODE_INVALID_OPCODE,
    ERROR_CODE_INVALID_OPCODE_TYPE,

    // 加载虚拟机代码错误
    ERROR_CODE_LOAD_OPCODE_ERROR,

    // 虚拟机执行失败
    ERROR_CODE_RUN_VM_ERROR,

    RUNTIME_ERR_OUT_OF_ARRAY_RANGE,
} ErrorCode;

struct ErrorMessageInfo {
    ErrorCode   error_code;
    std::string error_messaage;
};

typedef enum {
    ERROR_REPORT_TYPE_UNKNOW,
    ERROR_REPORT_TYPE_EXIT_NOW, // 立即退出
    ERROR_REPORT_TYPE_COLL_ERR, // 收集错误，等error的数量到达20, 然后再退出
} ErrorReportType;

struct ErrorReportContext {
    Package*        package;
    PackageUnit*    package_unit;

    std::string     source_file_name;
    std::string     line_content;
    unsigned int    line_number;
    unsigned int    column_number;

    std::string     error_message;
    std::string     advice;

    ErrorReportType report_type;


    char*           ring_compiler_file;
    unsigned int    ring_compiler_file_line;
    /*
     * ring_compiler_file
     * ring_compiler_file_line
     *     Ring 编译器开发者专用
     *     ring 编译器的源文件, 可以快速定位 error-report 所在的位置
     */
};

typedef enum {
    SYNTAX_VARIABLE_DEFINITION,

} SyntaxType;

struct SyntaxInfo {
    SyntaxType syntax_type;
    char*      bnf;
};

struct BinaryChunk {
};

#define MAX_DIMENSION_NUM (8)

/*
 * 默认初始化 64个 bucket
 * 每个bucket 有 1000 个 block
 */
#define MEM_MAX_BUCKET_NUM 4096
#define MEM_INIT_BUCKET_NUM 512

#define MEM_BLOCK_NUM 8
#define MEM_BLOCK_MAX_SIZE (MEM_MAX_BUCKET_NUM * 8)

#define MEM_ALIGN 8
#define MEM_ALIGN_LARGE 16

#define ROUND_UP8(n) \
    (((n) + MEM_ALIGN - 1) & ~(MEM_ALIGN - 1))

#define ROUND_UP16(n) \
    (((n) + MEM_ALIGN_LARGE - 1) & ~(MEM_ALIGN_LARGE - 1))

#define NULL_MEM_POOL nullptr

struct MemPool {
    char*                             name;

    std::vector<MemBlock*>            free_buckets;
    std::vector<MemBlock*>            active_buckets;

    std::unordered_map<void*, size_t> active_mem_map;

    size_t                            free_mem_size;
    size_t                            active_mem_size;

    size_t                            all_mem_size; // 整个生命周期都不变, 用于校验


    /*
     * free_buckets[0]: bucket size = 8;
     * free_buckets[1]: bucket size = 16;
     * free_buckets[2]: bucket size = 24;
     * free_buckets[3]: bucket size = 32;
     * ......
     * free_buckets[63]: bucket size = 512;
     */
};

struct MemBlock {
    void*     data;
    MemBlock* next;
};

/*
 * DEFINE_ERROR_REPORT_STR ring 语法报错str
 *
 * 这里使用的局部变量，每次都分配
 */
#define DEFINE_ERROR_REPORT_STR  \
    std::string compile_err_buf; \
    std::string compile_adv_buf;


#define str_eq(str1, str2) (strcmp((str1), (str2)) == 0)
#define str_eq_n(str1, str2, len) (strncmp((str1), (str2), (len)) == 0)

// clear terminal printf
#define STDOUT_CLEAR_SCREEN fprintf(stdout, "\e[1;1H\e[2J")
#define STDERR_CLEAR_SCREEN fprintf(stderr, "\e[1;1H\e[2J")

// move cursor to terminal (row, col) location
#define STDOUT_MOVE_CURSOR(row, col) fprintf(stdout, "%c[%d;%dH", 27, (row), (col))
#define STDERR_MOVE_CURSOR(row, col) fprintf(stderr, "%c[%d;%dH", 27, (row), (col))

#define LOG_COLOR_RED_UNDERLINE "\033[4;31m"
#define LOG_COLOR_GREEN_UNDERLINE "\033[4;32m"
#define LOG_COLOR_YELLOW_UNDERLINE "\033[4;33m"
#define LOG_COLOR_BLUE_UNDERLINE "\033[4;34m"
#define LOG_COLOR_PURPLE_UNDERLINE "\033[4;35m"

#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_YELLOW "\033[0;33m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_PURPLE "\033[0;35m"
#define LOG_COLOR_DARKGREEN "\033[0;36m"
#define LOG_COLOR_WHITE "\033[0;37m"

#define LOG_COLOR_CLEAR "\033[0m"

#define printf_witch_red(format, ...) \
    printf("%s" format "%s", LOG_COLOR_RED, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_green(format, ...) \
    printf("%s" format "%s", LOG_COLOR_GREEN, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_yellow(format, ...) \
    printf("%s" format "%s", LOG_COLOR_YELLOW, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_blue(format, ...) \
    printf("%s" format "%s", LOG_COLOR_BLUE, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_purple(format, ...) \
    printf("%s" format "%s", LOG_COLOR_PURPLE, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_darkgreen(format, ...) \
    printf("%s" format "%s", LOG_COLOR_DARKGREEN, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#define printf_witch_white(format, ...) \
    printf("%s" format "%s", LOG_COLOR_WHITE, ##__VA_ARGS__, LOG_COLOR_CLEAR)


#define complie_err_log(format, ...) \
    printf("%s" format "%s\n", LOG_COLOR_RED, ##__VA_ARGS__, LOG_COLOR_CLEAR)


// debug flex 词法分析
#ifdef DEBUG_FLEX
#define debug_flex_info_with_red(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_RED, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_flex_info_with_red(format, ...)
#endif

// debug bison 语法分析
#ifdef DEBUG_BISON
#define debug_bison_info_with_green(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_GREEN, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_bison_info_with_green(format, ...)
#endif

// debug 语法分析构建AST
#ifdef DEBUG_CREATE_AST
#define debug_ast_info_with_yellow(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_YELLOW, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_ast_info_with_yellow(format, ...)
#endif

// debug 生成 vm opcode
#ifdef DEBUG_GENERATE_DETAIL
#define debug_generate_info_with_darkgreen(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_DARKGREEN, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_generate_info_with_darkgreen(format, ...)
#endif

// debug ring debugger 的详情
#ifdef DEBUG_RDB_TRACE_DISPATH_DETAIL
#define debug_rdb_with_darkgreen(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_DARKGREEN, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_rdb_with_darkgreen(format, ...)
#endif

#ifdef DEBUG_EXEC_VM
#define debug_exec_info_with_white(format, ...) \
    printf("%s[DEBUG][%s:%d][function:%s]" format "%s\n", LOG_COLOR_WHITE, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_exec_info_with_white(format, ...)
#endif


#ifdef DEBUG_RVM_HEAP_ALLOC
#define debug_rvm_heap_alloc_with_green(format, ...) \
    printf("%s [DEBUG][%s:%d][function:%s] " format " %s\n", LOG_COLOR_WHITE, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, LOG_COLOR_CLEAR)
#else
#define debug_rvm_heap_alloc_with_green(format, ...)
#endif

// 以后通用的错误提示统一使用这个
#define ring_error_report(format, ...)                                          \
    fprintf(stderr, "%s[ERROR][%s:%d]\t " format "%s\n",                        \
            LOG_COLOR_RED, __FILE__, __LINE__, ##__VA_ARGS__, LOG_COLOR_CLEAR); \
    exit(1);

#define ring_exec_err_report(format, ...)                                       \
    fprintf(stderr, "%s[Ring Core Panic][%s:%d]\n" format "%s\n",               \
            LOG_COLOR_RED, __FILE__, __LINE__, ##__VA_ARGS__, LOG_COLOR_CLEAR); \
    exit(1);


#define DEBUG_RVM_INTERACTIVE_STDOUT_FILE "/tmp/ring-debug-vm.stdout.log"


int              cmd_handler_run(Ring_Command_Arg command_arg);
int              cmd_handler_build(Ring_Command_Arg command_arg);
int              cmd_handler_dump(Ring_Command_Arg command_arg);
int              cmd_handler_rdb(Ring_Command_Arg command_arg);
int              cmd_handler_version(Ring_Command_Arg command_arg);
int              cmd_handler_man(Ring_Command_Arg command_arg);
int              cmd_handler_help(Ring_Command_Arg command_arg);

ExecuterEntry*   ring_compile_main(std::string              input_file_name,
                                   std::vector<std::string> shell_args);
int              ring_exec(Ring_VirtualMachine* rvm, ExecuterEntry* executer_entry);

RVM_DebugConfig* new_debug_config(Ring_Command_Arg args);
int              enable_debug_config(RVM_DebugConfig* debug_config, Ring_Command_Arg args);
int              register_debugger(Ring_VirtualMachine* rvm, RVM_DebugConfig* debug_config);


/* --------------------
 * string.cpp
 * function definition
 *
 */
Ring_String* new_ring_string();
void         reset_ring_string(Ring_String* string);
void         ring_string_add_char(Ring_String* string, char ch);
char*        get_ring_string(Ring_String* string);
void         init_string_literal_buffer();
void         reset_string_literal_buffer();
void         string_literal_add_char(char ch);
char*        get_string_literal();
// --------------------


/* --------------------
 * compiler.cpp
 * function definition
 *
 */
void           init_front_mem_pool();
MemPool*       get_front_mem_pool();
void           destory_front_mem_pool();

const char*    get_string_constant_literal(const char* str1, const char* str2);

CompilerEntry* compiler_entry_create();
CompilerEntry* get_compiler_entry();
void           compiler_entry_dump(CompilerEntry* compiler_entry);
Package*       search_package(CompilerEntry* compiler_entry, char* package_name);
ExecuterEntry* executer_entry_create();
void           executer_entry_dump(ExecuterEntry* executer_entry);
Package*       package_create(CompilerEntry* compiler_entry, char* package_name, char* package_path);
Package*       package_create_input_file(CompilerEntry* compiler_entry, char* package_name, char* input_main_file);
void           package_compile(Package* package);
void           package_dump(Package* package);

PackageUnit*   package_unit_create(Package* parent_package, std::string file_name);
PackageUnit*   get_package_unit();
void           package_unit_compile(PackageUnit* package_unit);
void           package_unit_dump(PackageUnit* package_unit);
std::string    package_unit_get_file_name();
Ring_String*   get_package_unit_current_line_content();
unsigned int   package_unit_get_line_number();
unsigned int   package_unit_increa_line_number();
unsigned int   package_unit_get_column_number();
unsigned int   package_unit_increa_column_number(unsigned int len);
void           package_unit_update_line_content(char* str);
void           package_unit_reset_line_content();
char*          package_unit_get_current_line_content();
void           package_unit_reset_column_number();
std::string    package_unit_get_line_content(unsigned int line_number);

int            package_unit_add_enum_definition(EnumDeclaration* enum_decl);
int            package_unit_add_class_definition(ClassDefinition* class_definition);
int            package_unit_add_type_alias(TypeAlias* type_alias);

void           ring_grammar_error(RING_GRAMMAR_ID grammar_id);
int            yyerror(char const* str, ...);

void           ring_compile_error_report(ErrorReportContext* context);
void           ring_check_exit_immediately();
std::string    trans_flex_token_to_human_char(std::string str);
std::string    str_replace_all(std::string        subject,
                               const std::string& search,
                               const std::string& replace);
void           yyin_move_to_next_line();
// --------------------


/* --------------------
 * create_ast.cpp
 * function definition
 *
 */
void                          finish_global_block(Block* block, Statement* global_statement_list);

Statement*                    statement_list_add_item(Statement* statement_list, Statement* statement);
Statement*                    create_statemen_from_expression(Expression* expression);
void                          add_function_definition(AttributeInfo* attribute_info, Function* function_definition);

Expression*                   expression_add_package_posit(Expression* expression, char* package_posit);
Expression*                   create_expression_identifier(char* identifier);
Expression*                   create_expression_identifier_with_index(Expression* array_expression, DimensionExpression* index);
Expression*                   create_member_expression(Expression* object_expression, char* member_identifier);
Expression*                   create_expression_from_function_call(FunctionCallExpression* function_call_expression);
Expression*                   create_expression_from_member_call(MemberCallExpression* member_call_expression);
Expression*                   create_expression_from_array_literal(ArrayLiteralExpression* array_literal);
Expression*                   create_expression_from_class_object_literal(ClassObjectLiteralExpression* object_literal);
Expression*                   create_expression_from_slice_expression(SliceExpression* slice_expr);

Expression*                   create_expression_assign(AssignExpression* assign_expression);
Expression*                   create_expression_ternary(Expression* condition, Expression* true_expression, Expression* false_expression);
Expression*                   create_expression_launch(LaunchExpressionType          type,
                                                       FunctionCallExpression*       function_call_expression,
                                                       MemberCallExpression*         member_call_expression,
                                                       ImmediateInvokFuncExpression* iife);

Expression*                   create_expression_anonymous_func(AnonymousFunc* func);

ImmediateInvokFuncExpression* create_expression_iife(AnonymousFunc* closure,
                                                     ArgumentList*  argument_list);
Expression*                   create_expression_from_iife(ImmediateInvokFuncExpression* iife);

Expression*                   create_expression_binary(ExpressionType type, Expression* left, Expression* right);
Expression*                   create_expression_unitary(ExpressionType type, Expression* unitary_expression);
Expression*                   create_expression_literal(ExpressionType type, char* literal_interface);
Expression*                   create_expression_bool_literal(ExpressionType type, bool value);
Expression*                   create_cast_expression(TypeSpecifier* cast_type, Expression* operand);
Expression*                   create_new_array_expression(TypeSpecifier*       sub_type,
                                                          DimensionExpression* dimension_expression);

FieldInitExpression*          create_field_init_expression(char* field_identifier, Expression* init_expression);
FieldInitExpression*          field_init_list_add_item(FieldInitExpression* list, FieldInitExpression* item);
AssignExpression*             create_assign_expression(AssignExpressionType type, Expression* left, Expression* operand);

FunctionCallExpression*       create_function_call_expression(Expression*   func_expression,
                                                              ArgumentList* argument_list);
MemberCallExpression*         create_member_call_expression(Expression* object_expression, char* member_identifier, ArgumentList* argument_list);
ArrayLiteralExpression*       create_array_literal_expression(TypeSpecifier* sub_type, DimensionExpression* dimension_expression, Expression* expression_list);
ClassObjectLiteralExpression* create_class_object_literal_expression(TypeSpecifier* type_specifier, FieldInitExpression* field_init_expression_list);
Expression*                   expression_list_add_item(Expression* expression_list, Expression* expression);
ArgumentList*                 argument_list_add_item(ArgumentList* argument_list, ArgumentList* argument);
ArgumentList*                 create_argument_list_from_expression(Expression* expression);
Identifier*                   create_identifier(IdentifierType type, char* name);
Identifier*                   identifier_list_add_item(Identifier* identifier_list, Identifier* identifier);
FunctionReturnList*           create_function_return_list(TypeSpecifier* type_specifier);
FunctionReturnList*           function_return_list_add_item(FunctionReturnList* return_list,
                                                            TypeSpecifier*      type_specifier);

FunctionTuple*                create_function_tuple(Location*           location,
                                                    Parameter*          parameter_list,
                                                    FunctionReturnList* return_list,
                                                    Block*              block);
Function*                     create_function_definition(FunctionType type, Identifier* identifier, Parameter* parameter_list, FunctionReturnList* return_list, Block* block);

Statement*                    create_statement_from_if(IfStatement* if_statement);
IfStatement*                  create_if_statement(Expression* expression, Block* if_block, ElseIfStatement* elseif_statement_list, Block* else_block);
ElseIfStatement*              create_elseif_statement(Expression* expression, Block* elseif_block);
ElseIfStatement*              elseif_statement_add_item(ElseIfStatement* list, ElseIfStatement* elseif_statement);
Statement*                    create_statement_from_for(ForStatement* for_statement);
ForStatement*                 create_for_ternary_statement(Expression* init_expression, Expression* condition_expression, Expression* post_expression, Block* block);
ForStatement*                 create_for_range_statement(Expression*      left,
                                                         RangeExpression* range_expr,
                                                         Block*           block);
Statement*                    create_statement_from_dofor(DoForStatement* dofor_statement);
DoForStatement*               create_dofor_statement(Expression* init_expression, Block* block, Expression* condition_expression, Expression* post_expression);
Statement*                    create_statement_from_break(BreakStatement* break_statement);
BreakStatement*               create_break_statement(char* literal_interface);
TagDefinitionStatement*       create_tag_definition_statement(char* identifier);
JumpTagStatement*             create_jump_tag_statement(char* identifier);
DeferStatement*               create_defer_statement(ImmediateInvokFuncExpression* iife);
Statement*                    create_statement_from_continue(ContinueStatement* continue_statement);
ContinueStatement*            create_continue_statement();
Statement*                    create_statement_from_return(ReturnStatement* return_statement);
Statement*                    create_statement_from_tag_definition(TagDefinitionStatement* tag_def);
Statement*                    create_statement_from_jump_tag(JumpTagStatement* jump_tag_statement);
Statement*                    create_statement_from_defer(DeferStatement* defer_statement);

ReturnStatement*              create_return_statement(Expression* expression);

Block*                        start_new_block();
Block*                        finish_block(Block* block, Statement* statement_list);

DimensionExpression*          create_dimension_expression(SubDimensionExpression* dimension_list);
DimensionExpression*          create_dimension_expression_with_exclam(char* dimension_literal);
SubDimensionExpression*       create_sub_dimension_expression(Expression* num_expression);
SubDimensionExpression*       sub_dimension_expression_list_add_item(SubDimensionExpression* list, SubDimensionExpression* item);

SliceExpression*              create_slice_expression(Expression* operand, SubSliceExpression* sub_slice_expression);
SubSliceExpression*           create_sub_slice_expression(Expression* start_expr, Expression* end_expr);

RangeExpression*              create_range_expression_from_step_range(SubStepRangeExpression* sub_step_range);
RangeExpression*              create_range_expression_from_linear_range(SubLinearRangeExpression* sub_linear_range);

SubStepRangeExpression*       create_step_range_expression(Expression* start_expr,
                                                           Expression* end_expr,
                                                           Expression* step_expr,
                                                           bool        is_inclusive);
SubLinearRangeExpression*     create_linear_range_expression(Expression* collection_expr);


TypeSpecifier*                create_type_specifier(Ring_BasicType basic_type);
TypeSpecifier*                create_type_specifier_array(TypeSpecifier* sub_type, DimensionExpression* dimension);
TypeSpecifier*                create_type_specifier_alias(char* package_posit, char* identifier);

TypeAlias*                    add_type_alias_class(char*            class_identifier,
                                                   ClassDefinition* class_def);
TypeAlias*                    add_type_alias_func(Parameter*          parameter_list,
                                                  FunctionReturnList* return_list,
                                                  Identifier*         identifier);
TypeAlias*                    add_type_alias_enum(Identifier* identifier, EnumDeclaration* enum_declaration);

Ring_DeriveType_Func*         create_derive_type_func(Parameter*          parameter_list,
                                                      FunctionReturnList* return_list);

VarDecl*                      create_declaration(TypeSpecifier* type, char* identifier, Expression* initializer);
VarDecl*                      declaration_list_add_item(VarDecl* head, VarDecl* declaration);

Statement*                    create_multi_declaration_statement(TypeSpecifier* type_specifier, Identifier* identifier_list, Expression* initializer_list);

Parameter*                    create_parameter(TypeSpecifier* type, char* identifier, bool is_variadic);
Parameter*                    parameter_list_add_statement(Parameter* head, Parameter* parameter);

Package*                      create_package_info(Identifier* identifier);
void                          import_package_list_add_item(char* package_name, char* rename);

EnumDeclaration*              start_enum_declaration(TypeSpecifier* type_specifier, Identifier* identifier);
EnumDeclaration*              finish_enum_declaration(EnumDeclaration*     enum_decl,
                                                      EnumItemDeclaration* enum_item_decl);
EnumItemDeclaration*          enum_item_declaration_list_add_item(EnumItemDeclaration* list,
                                                                  EnumItemDeclaration* decl);
EnumItemDeclaration*          create_enum_item_declaration(char* identifier, Expression* value_expr);

ClassDefinition*              start_class_definition(char* class_identifier);
ClassDefinition*              finish_class_definition(ClassDefinition*        class_def,
                                                      ClassMemberDeclaration* class_member_declar,
                                                      char*                   class_identifier);

ClassMemberDeclaration*       class_member_declaration_list_add_item(ClassMemberDeclaration* list, ClassMemberDeclaration* decl);
ClassMemberDeclaration*       create_class_member_field_declaration(Attribute attribute, FieldMember* field_member);
ClassMemberDeclaration*       create_class_member_method_declaration(Attribute attribute, MethodMember* method_member);
FieldMember*                  create_class_member_field(TypeSpecifier* type_specifier, Identifier* identifier_list);
MethodMember*                 create_class_member_method(FunctionType type, Identifier* identifier, Parameter* parameter_list, FunctionReturnList* return_list, Block* block);


AttributeInfo*                create_attribute_info(char* name);
AttributeInfo*                attribute_info_add_item(AttributeInfo* list, AttributeInfo* item);

Attribute                     add_attribute(Attribute attribute, AttributeType type);
int                           attribute_is_public(Attribute attribute);
int                           attribute_is_private(Attribute attribute);
int                           attribute_is_constructor(Attribute attribute);
int                           attribute_is_destructor(Attribute attribute);

Location*                     a_location();
// --------------------


/* --------------------
 * semantic_check.cpp
 * function definition
 *
 */
void ring_compiler_semantic_analysis(Package* package);

void ring_compiler_analysis_import_package(Package* package);
void ring_compiler_analysis_global_variable(Package* package);
void ring_compiler_analysis_function(Package* package);
void ring_compiler_analysis_function_block(Package* package, Block* block);
void ring_compiler_analysis_class(Package* package);

void check_function_call(FunctionCallExpression* function_call_expression,
                         Function*               function);
void check_func_var_call(FunctionCallExpression* function_call_expression,
                         TypeSpecifier*          func_type_specifier);
void check_iife_call(ImmediateInvokFuncExpression* iife);
void check_method_call(MemberCallExpression* member_call_expression,
                       MethodMember*         method);
void check_field_call(MemberCallExpression* member_call_expression,
                      FieldMember*          field);
void check_call(char*          func_identifier,
                unsigned int   line_number,
                ArgumentList*  argument_list,
                Function*      function,
                TypeSpecifier* func_type_specifier,
                AnonymousFunc* anony_func,
                MethodMember*  method_member);

void ring_compiler_analysis_class_block(Package* package, ClassDefinition* class_def);

void ring_compiler_check_exit(Package* package);
// --------------------


/* --------------------
 * fix.cpp
 * function definition
 *
 */
void                        ring_compiler_fix_ast(Package* package);
Function*                   create_global_init_func(Package* package);
void                        fix_function_definition(Function* func);
void                        fix_function_block(Function* func);
void                        fix_statement_list(Statement* statement_list, Block* block, FunctionTuple* func);
void                        fix_statement(Statement* statement, Block* block, FunctionTuple* func);
void                        fix_expression(Expression* expression,
                                           Block* block, FunctionTuple* func,
                                           RingContext ctx);
void                        add_local_declaration(VarDecl* declaration, Block* block, FunctionTuple* func);
void                        fix_enum_declaration(EnumDeclaration* declaration, Package* curr_package, RingContext ctx);
void                        fix_type_specfier(TypeSpecifier* type_specifier, Package* curr_package);
void                        fix_block(Block* block, FunctionTuple* func);
void                        fix_if_statement(IfStatement* if_statement, Block* block, FunctionTuple* func);
void                        fix_for_statement(ForStatement* for_statement, Block* block, FunctionTuple* func);
void                        fix_for_range_statement(ForRangeStatement* for_range_statement,
                                                    Block*             block,
                                                    FunctionTuple*     func);

void                        fix_range_expression(RangeExpression* range_expression,
                                                 Block*           block,
                                                 FunctionTuple*   func);
void                        fix_dofor_statement(DoForStatement* dofor_statement, Block* block, FunctionTuple* func);
void                        fix_break_statement(BreakStatement* break_statement, Block* block, FunctionTuple* func);
void                        fix_continue_statement(ContinueStatement* continue_statement, Block* block, FunctionTuple* func);
void                        fix_return_statement(ReturnStatement* return_statement, Block* block, FunctionTuple* func);
void                        fix_defer_statement(DeferStatement* defer_statement, Block* block, FunctionTuple* func);
void                        fix_identifier_expression(Expression*           expression,
                                                      IdentifierExpression* identifier_expression,
                                                      Block*                block,
                                                      RingContext           ctx);
void                        fix_assign_expression(AssignExpression* expression, Block* block, FunctionTuple* func);
void                        fix_binary_concat_expression(Expression*       expression,
                                                         BinaryExpression* binary_expression,
                                                         Block* block, FunctionTuple* func);
void                        fix_binary_math_expression(Expression*       expression,
                                                       ExpressionType    expression_type,
                                                       BinaryExpression* binary_expression,
                                                       Block* block, FunctionTuple* func,
                                                       RingContext ctx);
void                        fix_binary_logical_expression(Expression*       expression,
                                                          ExpressionType    expression_type,
                                                          BinaryExpression* binary_expression,
                                                          Block* block, FunctionTuple* func,
                                                          RingContext ctx);
void                        fix_binary_relational_expression(Expression*       expression,
                                                             ExpressionType    expression_type,
                                                             BinaryExpression* binary_expression,
                                                             Block* block, FunctionTuple* func,
                                                             RingContext ctx);


void                        fix_bitwise_binary_expression(Expression*       expression,
                                                          ExpressionType    expression_type,
                                                          BinaryExpression* binary_expression,
                                                          Block* block, FunctionTuple* func,
                                                          RingContext ctx);
void                        fix_bitwise_unitary_not_expression(Expression* expression,
                                                               Expression* unitary_expression,
                                                               Block* block, FunctionTuple* func,
                                                               RingContext ctx);
void                        fix_unitary_minus_expression(Expression* expression,
                                                         Expression* unitary_expression,
                                                         Block* block, FunctionTuple* func,
                                                         RingContext ctx);
void                        fix_unitary_not_expression(Expression* expression,
                                                       Expression* unitary_expression,
                                                       Block* block, FunctionTuple* func,
                                                       RingContext ctx);
void                        fix_unitary_increase_decrease_expression(Expression* expression,
                                                                     Expression* unitary_expression,
                                                                     Block* block, FunctionTuple* func);

void                        fix_function_call_expression(Expression*             expression,
                                                         FunctionCallExpression* function_call_expression,
                                                         Block*                  block,
                                                         FunctionTuple*          func);
void                        fix_member_call_expression(Expression*           expression,
                                                       MemberCallExpression* member_call_expression,
                                                       Block*                block,
                                                       FunctionTuple*        func);
void                        fix_class_definition(ClassDefinition* class_definition);
void                        fix_class_field(ClassDefinition* class_definition, FieldMember* field);
void                        fix_class_method(ClassDefinition* class_definition, MethodMember* method);


void                        fix_array_index_expression(Expression*           expression,
                                                       ArrayIndexExpression* array_index_expression,
                                                       Block*                block,
                                                       FunctionTuple*        func);
void                        fix_slice_expression(Expression*      expression,
                                                 SliceExpression* slice_expression,
                                                 Block*           block,
                                                 FunctionTuple*   func);
void                        fix_new_array_expression(Expression* expression, NewArrayExpression* new_array_expression, Block* block, FunctionTuple* func);
void                        fix_dimension_expression(DimensionExpression* dimension_expression, Block* block, FunctionTuple* func);
void                        fix_array_literal_expression(Expression* expression, ArrayLiteralExpression* array_literal_expression, Block* block, FunctionTuple* func);

void                        fix_class_object_literal_expression(Expression* expression, ClassObjectLiteralExpression* literal_expression, Block* block, FunctionTuple* func);
void                        fix_field_member_expression(Expression* expression, MemberExpression* member_expression, Block* block, FunctionTuple* func);
void                        fix_class_member_expression(MemberExpression* member_expression, Expression* object_expression, char* member_identifier);
ClassDefinition*            search_class_definition(char* class_identifier);
TypeAlias*                  search_type_alias(Package* package, char* identifier);
FieldMember*                search_class_field(ClassDefinition* class_definition, char* identifier);
MethodMember*               search_class_method(ClassDefinition* class_definition, char* identifier);

void                        fix_ternary_condition_expression(Expression*        expression,
                                                             TernaryExpression* ternary_expression,
                                                             Block*             block,
                                                             FunctionTuple*     func);
void                        fix_launch_expression(Expression*       expression,
                                                  LaunchExpression* launch_expression,
                                                  Block*            block,
                                                  FunctionTuple*    func);

void                        fix_anonymous_func_expression(Expression*              expression,
                                                          AnonymousFuncExpression* closure_expression,
                                                          Block*                   block,
                                                          FunctionTuple*           func);
void                        fix_iife_expression(Expression*                   expression,
                                                ImmediateInvokFuncExpression* iife,
                                                Block*                        block,
                                                FunctionTuple*                func);

void                        add_parameter_to_declaration(Parameter* parameter, Block* block);


EnumDeclaration*            resolve_enum_decl(char*    enum_identifier,
                                              Block*   block,
                                              Package* current_package);
EnumItemDeclaration*        resolve_enum_item(EnumDeclaration* enum_decl, char* enum_item_identifier);

Package*                    resolve_package(char*        package_posit,
                                            unsigned int line_number,
                                            Block*       block,
                                            Package*     current_package,
                                            RingContext  ctx);
Variable*                   resolve_variable(Package* package, char* identifier, Block* block);
Variable*                   resolve_variable_global(Package* package, char* identifier, Block* block);
Variable*                   resolve_variable_recur(Package* package, char* identifier, Block* block);

Function*                   search_function(Package* package, char* identifier);

FreeValueDesc*              free_value_list_add_item(FreeValueDesc* head, FreeValueDesc* free_value);
Variable*                   variable_list_add_item(Variable* head, Variable* variable);

std::vector<TypeSpecifier*> get_type_specifier_from_expression_list(Expression* expression);
std::vector<TypeSpecifier*> get_type_specifier_from_range_expression(RangeExpression* expression);

// --------------------

/* --------------------
 * crop_ast.cpp
 * function definition
 *
 */

void crop_if_statement(IfStatement* if_statement, Block* block, FunctionTuple* func);
void crop_for_statement(ForStatement* for_statement, Block* block, FunctionTuple* func);

void crop_ternary_condition_expression(Expression*        expression,
                                       TernaryExpression* ternary_expression,
                                       Block*             block,
                                       FunctionTuple*     func);
void crop_binary_logical_expression(Expression*       expression,
                                    ExpressionType    expression_type,
                                    BinaryExpression* binary_expression,
                                    Block* block, FunctionTuple* func);
void crop_binary_relational_expression(Expression*       expression,
                                       ExpressionType    expression_type,
                                       BinaryExpression* binary_expression,
                                       Block* block, FunctionTuple* func);
void crop_binary_concat_expression(Expression*       expression,
                                   BinaryExpression* binary_expression,
                                   Block* block, FunctionTuple* func);
void crop_binary_match_expression(Expression*       expression,
                                  BinaryExpression* binary_expression,
                                  Block* block, FunctionTuple* func);
void crop_binary_bitwise_expression(Expression*       expression,
                                    BinaryExpression* binary_expression,
                                    Block* block, FunctionTuple* func);
void crop_unitary_expression(Expression* expression,
                             Expression* unitary_expression,
                             Block* block, FunctionTuple* func);
// --------------------


/* --------------------
 * generate.cpp
 * function definition
 *
 */
Package_Executer* package_executer_create(ExecuterEntry* executer_entry,
                                          char*          package_name,
                                          unsigned int   package_index);
void              print_package_executer(Package_Executer* package_executer);
void              package_executer_dump(Package_Executer* package_executer);

void              ring_generate_vm_code(Package* package, Package_Executer* executer);
void              ring_generate_vm_code(CompilerEntry* compiler_entry, ExecuterEntry* executer_entry);
void              add_global_variable(Package* package, Package_Executer* executer);
void              add_functions(Package* package, Package_Executer* executer);
void              add_classes(Package* package, Package_Executer* executer);
void              class_def_deep_copy(Package_Executer* executer, RVM_ClassDefinition* dst, ClassDefinition* src);

void              copy_function(Package_Executer* executer, RVM_Function* dst, Function* src);
void              copy_method(Package_Executer* executer, RVM_Method* dst, MethodMember* src);
void              copy_field(Package_Executer* executer, RVM_Field* dst, FieldMember* src);

void              add_top_level_code(Package* package, Package_Executer* executer);
void              generate_code_from_function_definition(Package_Executer* executer,
                                                         RVM_Function* dst, FunctionTuple* src);
void              generate_code_from_method_definition(Package_Executer* executer, RVM_Method* dst, MethodMember* src);
RVM_OpcodeBuffer* new_opcode_buffer();

void              generate_vmcode_from_block(Package_Executer* executer, Block* block, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_statement_list(Package_Executer* executer, Block* block, Statement* statement_list, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_if_statement(Package_Executer* executer, IfStatement* if_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_for_statement(Package_Executer* executer, ForStatement* for_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_for_ternary_statement(Package_Executer* executer, ForStatement* for_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_for_range_statement(Package_Executer* executer,
                                                           ForStatement*     for_statement,
                                                           RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_dofor_statement(Package_Executer* executer, DoForStatement* dofor_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_break_statement(Package_Executer* executer, Block* block, BreakStatement* break_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_continue_statement(Package_Executer* executer, Block* block, ContinueStatement* continue_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_return_statement(Package_Executer* executer, Block* block, ReturnStatement* return_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_jump_tag_statement(Package_Executer* executer, Block* block, JumpTagStatement* jump_tag_statement, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_defer_statement(Package_Executer* executer,
                                                       Block*            block,
                                                       DeferStatement*   defer_statement,
                                                       RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_expression(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_assign_expression(Package_Executer* executer, AssignExpression* expression, RVM_OpcodeBuffer* new_opcode_buffer);
void              generate_pop_to_leftvalue_reverse(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_pop_to_leftvalue(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_pop_to_leftvalue_identifier(Package_Executer* executer, IdentifierExpression* identifier_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_pop_to_leftvalue_member(Package_Executer* executer, MemberExpression* member_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_pop_to_leftvalue_array_index(Package_Executer* executer, ArrayIndexExpression* array_index_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_logical_expression(Package_Executer* executer, BinaryExpression* expression, RVM_OpcodeBuffer* opcode_buffer, RVM_Opcode opcode);
void              generate_vmcode_from_binary_expression(Package_Executer* executer,
                                                         ExpressionType    expression_type,
                                                         BinaryExpression* binary_expression,
                                                         RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_bitwise_binary_expression(Package_Executer* executer,
                                                                 ExpressionType    expression_type,
                                                                 BinaryExpression* binary_expression,
                                                                 RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_relational_expression(Package_Executer* executer,
                                                             ExpressionType    expression_type,
                                                             BinaryExpression* expression,
                                                             RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_bitwise_unitary_not_expression(Package_Executer* executer,
                                                                      Expression*       expression,
                                                                      RVM_OpcodeBuffer* opcode_buffer,
                                                                      RVM_Opcode        opcode);
void              generate_vmcode_from_unitary_minus_expression(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer, RVM_Opcode opcode);
void              generate_vmcode_from_unitary_not_expression(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer, RVM_Opcode opcode);
void              generate_vmcode_from_increase_decrease_expression(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_identifier_expression(Package_Executer* executer, IdentifierExpression* identifier_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_bool_expression(Package_Executer* executer, Expression* expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_int_expression(Package_Executer* executer,
                                                      Expression*       expression,
                                                      RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_int64_expression(Package_Executer* executer,
                                                        Expression*       expression,
                                                        RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_double_expression(Package_Executer* executer,
                                                         Expression*       expression,
                                                         RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_string_expression(Package_Executer* executer,
                                                         Expression*       expression,
                                                         RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_function_call_expression(Package_Executer* executer, FunctionCallExpression* function_call_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_member_call_expression(Package_Executer* executer, MemberCallExpression* member_call_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_cast_expression(Package_Executer* executer, CastExpression* cast_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_member_expression(Package_Executer* executer, MemberExpression* member_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_ternary_condition_expression(Package_Executer* executer, TernaryExpression* ternary_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_launch_expression(Package_Executer* executer,
                                                         LaunchExpression* launch_expression,
                                                         RVM_OpcodeBuffer* opcode_buffer);

void              deep_copy_closure(RVM_Function* dst, AnonymousFunc* src);
void              generate_vmcode_from_anonymous_func_expreesion(Package_Executer*        executer,
                                                                 AnonymousFuncExpression* closure_expression,
                                                                 RVM_OpcodeBuffer*        opcode_buffer);
void              generate_vmcode_from_iife_expreesion(Package_Executer*             executer,
                                                       ImmediateInvokFuncExpression* iife,
                                                       RVM_OpcodeBuffer*             opcode_buffer);

void              generate_vmcode_from_new_array_expression(Package_Executer*   executer,
                                                            NewArrayExpression* new_array_expression,
                                                            RVM_OpcodeBuffer*   opcode_buffer);
void              generate_vmcode_from_class_object_literal_expreesion(Package_Executer* executer, ClassObjectLiteralExpression* literal_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_array_literal_expreesion(Package_Executer* executer, ArrayLiteralExpression* array_literal_expression, RVM_OpcodeBuffer* opcode_buffer);
void              generate_vmcode_from_array_index_expression(Package_Executer*     executer,
                                                              ArrayIndexExpression* array_index_expression,
                                                              RVM_OpcodeBuffer*     opcode_buffer);
void              generate_vmcode_from_slice_expression(Package_Executer* executer,
                                                        SliceExpression*  slice_expression,
                                                        RVM_OpcodeBuffer* opcode_buffer);

void              generate_vmcode(Package_Executer* executer, RVM_OpcodeBuffer* opcode_buffer, RVM_Opcode opcode, unsigned long long operand, unsigned int line_number);

unsigned int      constant_pool_grow(Package_Executer* executer, unsigned int growth_size);
unsigned int      constant_pool_add_int(Package_Executer* executer, int int_literal);
unsigned int      constant_pool_add_int64(Package_Executer* executer, long long int64_literal);
unsigned int      constant_pool_add_double(Package_Executer* executer, double double_literal);
unsigned int      constant_pool_add_string(Package_Executer* executer, const char* string_literal);
unsigned int      constant_pool_add_closure(Package_Executer* executer, RVM_Function* func);

unsigned int      opcode_buffer_get_label(RVM_OpcodeBuffer* opcode_buffer);
void              opcode_buffer_set_label(RVM_OpcodeBuffer* opcode_buffer, unsigned int label, unsigned int label_address);
void              opcode_buffer_fix_label(RVM_OpcodeBuffer* opcode_buffer);
RVM_Opcode        convert_opcode_by_rvm_type(RVM_Opcode opcode, TypeSpecifier* type);
unsigned int      calc_runtime_stack_capacity(RVM_Byte* code_list, unsigned int code_size);
void              add_code_line_map(RVM_OpcodeBuffer* opcode_buffer, unsigned int line_number, unsigned int start_pc, unsigned int opcode_size);
void              dump_code_line_map(std::vector<RVM_SourceCodeLineMap>& code_line_map);
void              type_specifier_deep_copy(RVM_TypeSpecifier* dst, TypeSpecifier* src);
// --------------------


/* --------------------
 * execute.cpp
 * function definition
 *
 */

RVM_RuntimeStack*    new_runtime_stack();
RVM_RuntimeStatic*   new_runtime_static();
RVM_RuntimeHeap*     new_runtime_heap();

Ring_VirtualMachine* ring_virtualmachine_create();
void                 ring_virtualmachine_load_executer(Ring_VirtualMachine* rvm, ExecuterEntry* executer_entry);
void                 ring_virtualmachine_init(Ring_VirtualMachine* rvm);

void                 rvm_add_static_variable(Package_Executer* executer, RVM_RuntimeStatic* runtime_static);
void                 rvm_init_static_variable(Ring_VirtualMachine* rvm, Package_Executer* executer, RVM_RuntimeStatic* runtime_static);

int                  ring_execute_vm_code(Ring_VirtualMachine* rvm);

void                 invoke_native_function(Ring_VirtualMachine* rvm, RVM_Function* function, unsigned int argument_list_size);
void                 invoke_derive_function(Ring_VirtualMachine* rvm,
                                            RVM_ClassObject** caller_object, RVM_Function** caller_function, RVM_Closure** caller_closure,
                                            RVM_ClassObject* callee_object, RVM_Function* callee_function, RVM_Closure* callee_closure,
                                            unsigned int caller_resume_pc,
                                            unsigned int argument_list_size,
                                            bool         invoke_by_defer);
void                 derive_function_finish(Ring_VirtualMachine* rvm,
                                            RVM_ClassObject** caller_object, RVM_Function** caller_function, RVM_Closure** caller_closure,
                                            RVM_Function* callee_function,
                                            unsigned int  return_value_list_size);
RVM_Closure*         new_closure(Ring_VirtualMachine* rvm,
                                 RVM_Function* caller_function, RVM_Closure* caller_closure,
                                 RVM_Function* func);
RVM_CallInfo*        store_callinfo(RVM_CallInfo* head, RVM_CallInfo* call_info);
RVM_CallInfo*        restore_callinfo(RVM_CallInfo** head_);
void                 init_derive_function_local_variable(Ring_VirtualMachine* rvm,
                                                         RVM_ClassObject*     callee_object,
                                                         RVM_Function*        callee_function,
                                                         unsigned int         argument_list_size);


RVM_Array*           rvm_new_array_literal_bool(Ring_VirtualMachine* rvm, unsigned int size);
RVM_Array*           rvm_new_array_literal_int(Ring_VirtualMachine* rvm, unsigned int size);
RVM_Array*           rvm_new_array_literal_int64(Ring_VirtualMachine* rvm, unsigned int size);
RVM_Array*           rvm_new_array_literal_double(Ring_VirtualMachine* rvm, unsigned int size);
RVM_Array*           rvm_new_array_literal_string(Ring_VirtualMachine* rvm, unsigned int size);
RVM_Array*           rvm_new_array_literal_class_object(Ring_VirtualMachine* rvm,
                                                        unsigned int         size,
                                                        RVM_ClassDefinition* class_definition);
RVM_Array*           rvm_new_array_literal_closure(Ring_VirtualMachine* rvm,
                                                   unsigned int         size);
RVM_Array*           rvm_new_array_literal_a(Ring_VirtualMachine* rvm,
                                             Ring_BasicType       basic_type,
                                             unsigned int         dimension,
                                             unsigned int         size);

void                 rvm_array_get_length(Ring_VirtualMachine* rvm, RVM_Array* array, int* value);
void                 rvm_array_get_capacity(Ring_VirtualMachine* rvm, RVM_Array* array, int* value);

void                 rvm_string_get_length(Ring_VirtualMachine* rvm, RVM_String* string, int* value);
void                 rvm_string_get_capacity(Ring_VirtualMachine* rvm, RVM_String* string, int* value);

ErrorCode            rvm_array_get_array(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_Array** value);
ErrorCode            rvm_array_set_array(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_Array** value);
ErrorCode            rvm_array_append_array(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_Array** value);
ErrorCode            rvm_array_pop_array(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_Array** value);

ErrorCode            rvm_array_get_bool(Ring_VirtualMachine* rvm, RVM_Array* array, int index, bool* value);
ErrorCode            rvm_array_set_bool(Ring_VirtualMachine* rvm, RVM_Array* array, int index, bool* value);
ErrorCode            rvm_array_append_bool(Ring_VirtualMachine* rvm, RVM_Array* array, bool* value);
ErrorCode            rvm_array_pop_bool(Ring_VirtualMachine* rvm, RVM_Array* array, bool* value);

ErrorCode            rvm_array_get_int(Ring_VirtualMachine* rvm, RVM_Array* array, int index, int* value);
ErrorCode            rvm_array_set_int(Ring_VirtualMachine* rvm, RVM_Array* array, int index, int* value);
ErrorCode            rvm_array_append_int(Ring_VirtualMachine* rvm, RVM_Array* array, int* value);
ErrorCode            rvm_array_pop_int(Ring_VirtualMachine* rvm, RVM_Array* array, int* value);

ErrorCode            rvm_array_get_int64(Ring_VirtualMachine* rvm, RVM_Array* array, int index, long long* value);
ErrorCode            rvm_array_set_int64(Ring_VirtualMachine* rvm, RVM_Array* array, int index, long long* value);
ErrorCode            rvm_array_append_int64(Ring_VirtualMachine* rvm, RVM_Array* array, long long* value);
ErrorCode            rvm_array_pop_int64(Ring_VirtualMachine* rvm, RVM_Array* array, long long* value);


ErrorCode            rvm_array_get_double(Ring_VirtualMachine* rvm, RVM_Array* array, int index, double* value);
ErrorCode            rvm_array_set_double(Ring_VirtualMachine* rvm, RVM_Array* array, int index, double* value);
ErrorCode            rvm_array_append_double(Ring_VirtualMachine* rvm, RVM_Array* array, double* value);
ErrorCode            rvm_array_pop_double(Ring_VirtualMachine* rvm, RVM_Array* array, double* value);

ErrorCode            rvm_array_get_string(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_String** value);
ErrorCode            rvm_array_set_string(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_String** value);
ErrorCode            rvm_array_append_string(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_String** value);
ErrorCode            rvm_array_pop_string(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_String** value);

ErrorCode            rvm_array_get_class_object(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_ClassObject** value);
ErrorCode            rvm_array_set_class_object(Ring_VirtualMachine* rvm, RVM_Array* array, int index, RVM_ClassObject** value);
ErrorCode            rvm_array_append_class_object(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_ClassObject** value);
ErrorCode            rvm_array_pop_class_object(Ring_VirtualMachine* rvm, RVM_Array* array, RVM_ClassObject** value);

ErrorCode            rvm_array_get_closure(Ring_VirtualMachine* rvm,
                                           RVM_Array*           array,
                                           int                  index,
                                           RVM_Closure**        value);
ErrorCode            rvm_array_set_closure(Ring_VirtualMachine* rvm,
                                           RVM_Array*           array,
                                           int                  index,
                                           RVM_Closure**        value);
ErrorCode            rvm_array_append_closure(Ring_VirtualMachine* rvm,
                                              RVM_Array*           array,
                                              RVM_Closure**        value);
ErrorCode            rvm_array_pop_closure(Ring_VirtualMachine* rvm,
                                           RVM_Array*           array,
                                           RVM_Closure**        value);

RVM_String*          rvm_bool_2_string(Ring_VirtualMachine* rvm, bool value);
RVM_String*          rvm_int_2_string(Ring_VirtualMachine* rvm, int value);
RVM_String*          rvm_int64_2_string(Ring_VirtualMachine* rvm, long long value);
RVM_String*          rvm_double_2_string(Ring_VirtualMachine* rvm, double value);

int                  rvm_string_cmp(RVM_String* string1, RVM_String* string2);


RVM_DeferItem*       new_defer_item(Ring_VirtualMachine* rvm, RVM_Closure* closure, unsigned int argument_list_size);
void                 coroutine_push_defer_item(Ring_VirtualMachine* rvm, RVM_DeferItem* defer_item);
RVM_DeferItem*       coroutine_pop_defer_item(Ring_VirtualMachine* rvm);
void                 fill_defer_item_argument_stack(Ring_VirtualMachine* rvm, RVM_DeferItem* defer_item);

RVM_Array*           init_derive_function_variadic_argument(Ring_VirtualMachine* rvm,
                                                            RVM_Parameter*       parameter,
                                                            unsigned int         size);
void                 batch_set_variadic_element(Ring_VirtualMachine* rvm);
RVM_Array_Type       convert_rvm_array_type(RVM_TypeSpecifier* type_specifier);
// --------------------


/* --------------------
 * bytecode.cpp
 * function definition
 *
 */
void ring_bytecode_dump(Package_Executer* executer, FILE* output);
void ring_bytecode_load(Package_Executer* executer, FILE* input);
// --------------------


/* --------------------
 * std_lib.cpp
 * function definition
 *
 */

void       compile_std_lib(CompilerEntry* compiler_entry, ExecuterEntry* executer_entry);
void       register_lib(Package_Executer* package_executer, char* func_name, RVM_NativeFuncProc* func_proc, int arg_count, int return_list_count);

RVM_Value* new_native_return_list(unsigned int return_size);
void       destory_native_return_list(RVM_Value* return_list, unsigned int return_size);


void       std_lib_os_remove(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_os_rename(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_os_getenv(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_os_setenv(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_os_platform(Ring_VirtualMachine* rvm,
                               unsigned int arg_size, RVM_Value* args,
                               unsigned int* return_size, RVM_Value** return_list);

void       std_lib_io_exist(Ring_VirtualMachine* rvm,
                            unsigned int arg_size, RVM_Value* args,
                            unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_open(Ring_VirtualMachine* rvm,
                           unsigned int arg_size, RVM_Value* args,
                           unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_create(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_seek(Ring_VirtualMachine* rvm,
                           unsigned int arg_size, RVM_Value* args,
                           unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_read_all(Ring_VirtualMachine* rvm,
                               unsigned int arg_size, RVM_Value* args,
                               unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_write(Ring_VirtualMachine* rvm,
                            unsigned int arg_size, RVM_Value* args,
                            unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_close(Ring_VirtualMachine* rvm,
                            unsigned int arg_size, RVM_Value* args,
                            unsigned int* return_size, RVM_Value** return_list);
void       std_lib_io_remove(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);

void       std_lib_fmt_println_bool(Ring_VirtualMachine* rvm,
                                    unsigned int arg_size, RVM_Value* args,
                                    unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println_int(Ring_VirtualMachine* rvm,
                                   unsigned int arg_size, RVM_Value* args,
                                   unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println_int64(Ring_VirtualMachine* rvm,
                                     unsigned int arg_size, RVM_Value* args,
                                     unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println_double(Ring_VirtualMachine* rvm,
                                      unsigned int arg_size, RVM_Value* args,
                                      unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println_string(Ring_VirtualMachine* rvm,
                                      unsigned int arg_size, RVM_Value* args,
                                      unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println_pointer(Ring_VirtualMachine* rvm,
                                       unsigned int arg_size, RVM_Value* args,
                                       unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_println(Ring_VirtualMachine* rvm,
                               unsigned int arg_size, RVM_Value* args,
                               unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_printf(Ring_VirtualMachine* rvm,
                              unsigned int arg_size, RVM_Value* args,
                              unsigned int* return_size, RVM_Value** return_list);
void       std_lib_fmt_sprintf(Ring_VirtualMachine* rvm,
                               unsigned int arg_size, RVM_Value* args,
                               unsigned int* return_size, RVM_Value** return_list);

void       std_lib_debug_assert(Ring_VirtualMachine* rvm,
                                unsigned int arg_size, RVM_Value* args,
                                unsigned int* return_size, RVM_Value** return_list);
void       std_lib_debug_var_dump(Ring_VirtualMachine* rvm,
                                  unsigned int arg_size, RVM_Value* args,
                                  unsigned int* return_size, RVM_Value** return_list);

void       std_lib_encoding_json_encode(Ring_VirtualMachine* rvm,
                                        unsigned int arg_size, RVM_Value* args,
                                        unsigned int* return_size, RVM_Value** return_list);
void       std_lib_encoding_json_encode_indent(Ring_VirtualMachine* rvm,
                                               unsigned int arg_size, RVM_Value* args,
                                               unsigned int* return_size, RVM_Value** return_list);

void       std_lib_reflect_typeof(Ring_VirtualMachine* rvm,
                                  unsigned int arg_size, RVM_Value* args,
                                  unsigned int* return_size, RVM_Value** return_list);
void       std_lib_encoding_json_decode(Ring_VirtualMachine* rvm,
                                        unsigned int arg_size, RVM_Value* args,
                                        unsigned int* return_size, RVM_Value** return_list);

void       std_lib_runtime_heap_size(Ring_VirtualMachine* rvm,
                                     unsigned int arg_size, RVM_Value* args,
                                     unsigned int* return_size, RVM_Value** return_list);
void       std_lib_runtime_gc(Ring_VirtualMachine* rvm,
                              unsigned int arg_size, RVM_Value* args,
                              unsigned int* return_size, RVM_Value** return_list);
void       std_lib_runtime_print_call_stack(Ring_VirtualMachine* rvm,
                                            unsigned int arg_size, RVM_Value* args,
                                            unsigned int* return_size, RVM_Value** return_list);
void       std_lib_runtime_call_info(Ring_VirtualMachine* rvm,
                                     unsigned int arg_size, RVM_Value* args,
                                     unsigned int* return_size, RVM_Value** return_list);

void       std_lib_time_time(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_time_sleep(Ring_VirtualMachine* rvm,
                              unsigned int arg_size, RVM_Value* args,
                              unsigned int* return_size, RVM_Value** return_list);

void       std_lib_math_abs(Ring_VirtualMachine* rvm,
                            unsigned int arg_size, RVM_Value* args,
                            unsigned int* return_size, RVM_Value** return_list);
void       std_lib_math_sqrt(Ring_VirtualMachine* rvm,
                             unsigned int arg_size, RVM_Value* args,
                             unsigned int* return_size, RVM_Value** return_list);
void       std_lib_math_pow(Ring_VirtualMachine* rvm,
                            unsigned int arg_size, RVM_Value* args,
                            unsigned int* return_size, RVM_Value** return_list);
// --------------------

/* --------------------
 * std_lib_json.cpp
 * function definition
 *
 */

std::string rvm_value_json_encode(RVM_Value* value,
                                  const int  indent      = -1,
                                  const char indent_char = ' ');

RVM_Value   rvm_value_json_decode(Ring_VirtualMachine* rvm,
                                  char*                str,
                                  unsigned int         length,
                                  RVM_Value*           expect);

// 将 RVM_Value 转换为 nlohmann::json
json             rvm_value_2_json(RVM_Value* value);
json             rvm_class_ob_2_json(RVM_ClassObject* obj);
json             rvm_array_2_json(RVM_Array* arr);


RVM_Value        json_2_rvm_value(Ring_VirtualMachine* rvm,
                                  const json&          j,
                                  RVM_Value*           expect);
RVM_ClassObject* json_2_rvm_class_ob(Ring_VirtualMachine* rvm,
                                     const json&          j,
                                     RVM_ClassDefinition* rvm_class_definition);
RVM_Array*       json_2_rvm_array(Ring_VirtualMachine* rvm,
                                  const json&          j,
                                  unsigned int         expect_array_dimension,
                                  RVM_Array_Type       expect_array_type,
                                  Ring_BasicType       expect_array_item_type_kind,
                                  RVM_ClassDefinition* expect_array_class_def);
// --------------------

/* --------------------
 * utils.cpp
 * function definition
 *
 */
std::string              formate_expression(Expression* expression);
std::string              formate_operator(ExpressionType expression_type);

void                     ring_compiler_functions_dump(PackageUnit* package_unit);
void                     ring_vm_constantpool_dump(Package_Executer* executer);
void                     ring_vm_code_dump(RVM_Function* function, RVM_Byte* code_list, unsigned int code_size, unsigned int pc, unsigned int screen_row, unsigned int screen_col);
void                     ring_vm_dump_runtime_stack(Ring_VirtualMachine* rvm,
                                                    RVM_RuntimeStack*    runtime_stack,
                                                    unsigned int         caller_stack_base,
                                                    unsigned int screen_row, unsigned int screen_col);
void                     ring_vm_dump_stdout_log(Ring_VirtualMachine* rvm);
std::vector<std::string> list_files_of_dir(char* dir);
RingFileStat*            create_ring_file_stat(std::string& file_name);
std::vector<std::string> get_file_contents(RingFileStat* file_stat, unsigned int start_line, unsigned int end_line);
std::string              get_file_content(RingFileStat* file_stat, unsigned int line_number);

void                     dump_vm_function(Package_Executer*    package_executer,
                                          RVM_ClassDefinition* class_definition,
                                          RVM_Function*        function);
void                     dump_vm_class(Package_Executer*    package_executer,
                                       RVM_ClassDefinition* class_definition);
std::string              dump_vm_constant(RVM_Constant* constant);

unsigned int             get_source_line_number_by_pc(RVM_Function* function, unsigned int pc);

std::string              format_rvm_type(Ring_VirtualMachine* rvm, RVM_Value* value);
std::string              format_rvm_value(RVM_Value* value);
std::string              format_rvm_call_stack(Ring_VirtualMachine* rvm);
unsigned int             get_rvm_call_stack_level(Ring_VirtualMachine* rvm);
CallInfo                 get_rvm_call_stack(Ring_VirtualMachine* rvm, unsigned int skip);
std::string              format_rvm_current_func(Ring_VirtualMachine* rvm, unsigned int source_line_number);

std::string              format_type_specifier(std::vector<TypeSpecifier*> convert_type);
std::string              format_type_specifier(unsigned int convert_type_size, TypeSpecifier** convert_type);
std::string              format_type_specifier(TypeSpecifier* type_specifier);
std::string              format_type_specifier_array(Ring_DeriveType_Array* array_type);
std::string              format_type_specifier_class(Ring_DeriveType_Class* class_type);
std::string              format_type_specifier_func(Ring_DeriveType_Func* func_type);
std::string              format_function_parameters(Parameter* parameter);

std::string              format_function_return_list(FunctionReturnList* return_list);
std::string              format_function_arguments(ArgumentList* argument);

std::string              format_rvm_function_type(Package_Executer* package_executer,
                                                  RVM_Function*     function);
std::string              format_rvm_function(Package_Executer* package_executer,
                                             RVM_Function*     function);
std::string              format_rvm_type_specifier(Package_Executer*  package_executer,
                                                   RVM_TypeSpecifier* type_specifier,
                                                   std::string        prefix);

std::vector<std::string> split(const std::string& str, const std::string& delimiters);

std::vector<std::string> splitargs(const char* line);
static int               hex_digit_to_int(char c);

std::string              strings_join(const std::vector<std::string>& lst, const std::string& delim);
int                      string_compare(const char* str1, unsigned int str1_len, const char* str2, unsigned int str2_len);

std::string              formate_class_type(RVM_ClassObject* class_object);

std::string              formate_array_type(RVM_Array* array_value);
std::string              formate_array_item_type(RVM_Array* array_value);

std::string              formate_closure_type(Package_Executer* package_executer,
                                              RVM_Closure*      closure_value);

bool                     comp_type_specifier(TypeSpecifier* a, TypeSpecifier* b);
bool                     comp_type_specifier_func(Ring_DeriveType_Func* a, Ring_DeriveType_Func* b);
bool                     comp_type_specifier_dimension(TypeSpecifier* array, TypeSpecifier* item, unsigned int dimension);

std::string              sprintf_string(const char* format, ...);
std::string              sprintf_string_va(const char* format, va_list args);

std::string              convert_troff_string_2_c_control(const std::string& input);


// --------------------


/* --------------------
 * man_help.cpp
 * function definition
 *
 */
std::string get_help_message();
std::string get_man_help(const char* keyword);
// --------------------

/* --------------------
 * mem_pool.cpp
 * function definition
 *
 */
MemPool* create_mem_pool(char* name);
void     destory_mem_pool(MemPool* pool);

void     dump_mem_pool(MemPool* pool);
void*    mem_alloc(MemPool* pool, size_t size);
void*    mem_realloc(MemPool* pool, void* ptr, size_t old_size, size_t new_size);
void     mem_free(MemPool* pool, void* ptr, size_t size);
void     test_mem_pool();
// --------------------

/* --------------------
 * gc.cpp
 * function definition
 *
 */

void        rvm_heap_alloc_size_incr(Ring_VirtualMachine* rvm, long long size);
long long   rvm_heap_size(Ring_VirtualMachine* rvm);
void        rvm_heap_list_add_object(Ring_VirtualMachine* rvm, RVM_GC_Object* object);
void        rvm_heap_list_remove_object(Ring_VirtualMachine* rvm, RVM_GC_Object* object);


void        gc_incremental(Ring_VirtualMachine* rvm);
void        gc_make_sweep(Ring_VirtualMachine* rvm);
void        gc_mark(Ring_VirtualMachine* rvm);
void        gc_sweep(Ring_VirtualMachine* rvm);

void        gc_mark_rvm_value(RVM_Value* value);
void        gc_mark_class_ob(RVM_ClassObject* class_ob);
void        gc_mark_array(RVM_Array* array);
void        gc_mark_closure(RVM_Closure* closure);
void        gc_mark_fvb(RVM_FreeValueBlock* fvb);
void        gc_mark_free_value(RVM_FreeValue* free_value);

void        rvm_free_object(Ring_VirtualMachine* rvm, RVM_GC_Object* object);

RVM_String* new_string_meta();
// 不分配在 heap上
RVM_String*         string_literal_to_rvm_string(const char* string_literal);
RVM_String*         rvm_gc_new_rvm_string(Ring_VirtualMachine* rvm, const char* string_literal);
RVM_String*         rvm_gc_new_string_meta(Ring_VirtualMachine* rvm);
void                rvm_fill_string(Ring_VirtualMachine* rvm, RVM_String* string, unsigned int capacity);
RVM_String*         rvm_deep_copy_string(Ring_VirtualMachine* rvm, RVM_String* src);
RVM_String*         concat_string(Ring_VirtualMachine* rvm, RVM_String* a, RVM_String* b);
RVM_String*         rvm_slice_string(Ring_VirtualMachine* rvm,
                                     RVM_String*          src,
                                     long long            start,
                                     long long            end);
unsigned int        rvm_free_string(Ring_VirtualMachine* rvm, RVM_String* string);


RVM_Array*          rvm_gc_new_array_meta(Ring_VirtualMachine* rvm,
                                          RVM_Array_Type       array_type,
                                          Ring_BasicType       item_type_kind,
                                          RVM_ClassDefinition* class_definition,
                                          unsigned int         dimension);
RVM_Array*          rvm_new_array(Ring_VirtualMachine* rvm,
                                  unsigned int         dimension,
                                  unsigned int*        dimension_list,
                                  unsigned int         dimension_index,
                                  RVM_Array_Type       array_type,
                                  Ring_BasicType       item_type_kind,
                                  RVM_ClassDefinition* class_definition);
RVM_Array*          rvm_deep_copy_array(Ring_VirtualMachine* rvm, RVM_Array* src);
RVM_Array*          rvm_slice_array(Ring_VirtualMachine* rvm,
                                    RVM_Array*           src,
                                    long long            start,
                                    long long            end);
void                rvm_array_growth(Ring_VirtualMachine* rvm, RVM_Array* array);
unsigned int        rvm_free_array(Ring_VirtualMachine* rvm, RVM_Array* array);


RVM_ClassObject*    rvm_gc_new_class_ob_meta(Ring_VirtualMachine* rvm);
void                rvm_fill_class_ob(Ring_VirtualMachine* rvm,
                                      RVM_ClassObject*     class_ob,
                                      RVM_ClassDefinition* class_definition);
RVM_ClassObject*    rvm_deep_copy_class_ob(Ring_VirtualMachine* rvm, RVM_ClassObject* src);
unsigned int        rvm_free_class_ob(Ring_VirtualMachine* rvm, RVM_ClassObject* class_ob);


RVM_Closure*        rvm_gc_new_closure_meta(Ring_VirtualMachine* rvm);
void                rvm_fill_closure(Ring_VirtualMachine* rvm,
                                     RVM_Closure*         closure,
                                     RVM_Function*        callee_function);
RVM_Closure*        rvm_deep_copy_closure(Ring_VirtualMachine* rvm, RVM_Closure* src);
unsigned int        rvm_free_closure(Ring_VirtualMachine* rvm, RVM_Closure* closure);

RVM_FreeValueBlock* rvm_gc_new_fvb_meta(Ring_VirtualMachine* rvm);
void                rvm_fill_fvb(Ring_VirtualMachine* rvm,
                                 RVM_FreeValueBlock*  fvb,
                                 unsigned int         free_value_size);
unsigned int        rvm_free_fvb(Ring_VirtualMachine* rvm, RVM_FreeValueBlock* fvb);
// --------------------


/* --------------------
 * rdb-cli.cpp
 * rdb-dap.cpp
 * function definition
 *
 */

int dap_debug_trace_dispatch(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_sae(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_opcode(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_line(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_call(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_return(RVM_Frame* frame, const char* event, const char* arg);
int dap_dispath_exit(RVM_Frame* frame, const char* event, const char* arg);

std::variant<int, dap::LaunchRequest>
        dap_rdb_message_process_loop_norun(RVM_DebugConfig* debug_config);
int     dap_rdb_message_process_loop(RVM_Frame* frame, const char* event, const char* arg);


int     cli_debug_trace_dispatch(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_sae(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_opcode(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_line(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_call(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_return(RVM_Frame* frame, const char* event, const char* arg);
int     cli_dispath_exit(RVM_Frame* frame, const char* event, const char* arg);

RDB_Arg cli_rdb_message_process_loop_norun(RVM_DebugConfig* debug_config);
RDB_Arg cli_rdb_message_process_loop(RVM_Frame* frame, const char* event, const char* arg);
RDB_Arg rdb_parse_command(const char* line);

void    rdb_input_completion(const char* buf, linenoiseCompletions* lc);
char*   rdb_input_hints(const char* buf, int* color, int* bold);


// --------------------

/* --------------------
 * vm-debug.cpp
 * function definition
 *
 */

int       debug_rvm_interactive(Ring_VirtualMachine* rvm, RVM_Function* function, RVM_Byte* code_list, unsigned int code_size, unsigned int pc, unsigned int caller_stack_base);
int       vm_debugger_cli(Ring_VirtualMachine* rvm);
VM_DB_Arg vm_db_parse_command(const char* line);
// --------------------

/* --------------------
 * debug-free-value.cpp
 * function definition
 *
 */
void debug_generate_closure_dot_file(RVM_Closure* closure);
// --------------------

/* --------------------
 * debug-gc.cpp
 * function definition
 *
 */
void debug_gc_summary(Ring_VirtualMachine* rvm, std::string stage);
// --------------------

/* --------------------
 * vm.cpp
 * function definition
 *
 */
int                             rvm_function_calc_stack_cap(RVM_Function* function);
int                             opcode_calc_stack_cap(RVM_Byte* code_list, unsigned int pc, RVM_Byte opcode);
std::vector<_StackIncrExprTerm> parse_stack_incr_expr(const char* expression);
// --------------------


/* --------------------
 * buildin.cpp
 * function definition
 *
 */


void         check_build_func_param_num(Expression*             expression,
                                        FunctionCallExpression* function_call_expression,
                                        Block*                  block,
                                        Function*               func,
                                        Ring_Buildin_Func*      build_func);

unsigned int is_buildin_function_identifier(char* package_posit, char* identifier);
void         fix_buildin_func(Expression*             expression,
                              FunctionCallExpression* function_call_expression,
                              Block*                  block,
                              FunctionTuple*          func);
void         generate_vmcode_buildin_func(Package_Executer*       executer,
                                          FunctionCallExpression* function_call_expression,
                                          RVM_OpcodeBuffer*       opcode_buffer);

void         fix_buildin_func_len(Expression*             expression,
                                  FunctionCallExpression* function_call_expression,
                                  Block*                  block,
                                  Function*               func,
                                  Ring_Buildin_Func*      build_func);
void         fix_buildin_func_cap(Expression*             expression,
                                  FunctionCallExpression* function_call_expression,
                                  Block*                  block,
                                  Function*               func,
                                  Ring_Buildin_Func*      build_func);
void         fix_buildin_func_push(Expression*             expression,
                                   FunctionCallExpression* function_call_expression,
                                   Block*                  block,
                                   Function*               func,
                                   Ring_Buildin_Func*      build_func);
void         fix_buildin_func_pop(Expression*             expression,
                                  FunctionCallExpression* function_call_expression,
                                  Block*                  block,
                                  Function*               func,
                                  Ring_Buildin_Func*      build_func);
void         fix_buildin_func_to_string(Expression*             expression,
                                        FunctionCallExpression* function_call_expression,
                                        Block*                  block,
                                        Function*               func,
                                        Ring_Buildin_Func*      build_func);
void         fix_buildin_func_to_int64(Expression*             expression,
                                       FunctionCallExpression* function_call_expression,
                                       Block*                  block,
                                       Function*               func,
                                       Ring_Buildin_Func*      build_func);

void         fix_buildin_func_to_resume(Expression*             expression,
                                        FunctionCallExpression* function_call_expression,
                                        Block*                  block,
                                        Function*               func,
                                        Ring_Buildin_Func*      build_func);
void         fix_buildin_func_to_yield(Expression*             expression,
                                       FunctionCallExpression* function_call_expression,
                                       Block*                  block,
                                       Function*               func,
                                       Ring_Buildin_Func*      build_func);
void         fix_buildin_func_to_os_exit(Expression*             expression,
                                         FunctionCallExpression* function_call_expression,
                                         Block*                  block,
                                         Function*               func,
                                         Ring_Buildin_Func*      build_func);


void         generate_buildin_func_len(Package_Executer*       executer,
                                       FunctionCallExpression* function_call_expression,
                                       RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_cap(Package_Executer*       executer,
                                       FunctionCallExpression* function_call_expression,
                                       RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_push(Package_Executer*       executer,
                                        FunctionCallExpression* function_call_expression,
                                        RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_pop(Package_Executer*       executer,
                                       FunctionCallExpression* function_call_expression,
                                       RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_to_string(Package_Executer*       executer,
                                             FunctionCallExpression* function_call_expression,
                                             RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_to_int64(Package_Executer*       executer,
                                            FunctionCallExpression* function_call_expression,
                                            RVM_OpcodeBuffer*       opcode_buffer);
void         generate_buildin_func_resume(Package_Executer*       executer,
                                          FunctionCallExpression* function_call_expression,
                                          RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_yield(Package_Executer*       executer,
                                         FunctionCallExpression* function_call_expression,
                                         RVM_OpcodeBuffer*       opcode_buffer);

void         generate_buildin_func_os_exit(Package_Executer*       executer,
                                           FunctionCallExpression* function_call_expression,
                                           RVM_OpcodeBuffer*       opcode_buffer);


// --------------------


/* --------------------
 * fmt.cpp
 * function definition
 *
 */
std::string fmt_any(RVM_Value* value);
std::string fmt_bool(RVM_Value* value);
std::string fmt_int(RVM_Value* value);
std::string fmt_int64(RVM_Value* value);
std::string fmt_double(RVM_Value* value);
std::string fmt_string(RVM_String* string_value);
std::string fmt_class(RVM_ClassObject* class_object);
std::string fmt_array(RVM_Array* array_value);
std::string fmt_closure(RVM_Closure* closure);

std::string var_dump_any(Package_Executer* package_executer,
                         RVM_Value*        value,
                         unsigned int      indent);
std::string var_dump_bool(RVM_Value* value);
std::string var_dump_int(RVM_Value* value);
std::string var_dump_int64(RVM_Value* value);
std::string var_dump_double(RVM_Value* value);
std::string var_dump_string(RVM_String* string_value);
std::string var_dump_class(Package_Executer* package_executer,
                           RVM_ClassObject*  class_object,
                           unsigned int      indent);
std::string var_dump_array(Package_Executer* package_executer,
                           RVM_Array*        array_value,
                           unsigned int      indent);
std::string var_dump_closure(Package_Executer* package_executer, RVM_Closure* closure);

std::string build_indent(unsigned int indent, std::string indent_str);


// --------------------


/* --------------------
 * coroutine.cpp
 * function definition
 *
 */

RingCoroutine* launch_root_coroutine(Ring_VirtualMachine* rvm);
RingCoroutine* launch_coroutine(Ring_VirtualMachine* rvm,
                                RVM_ClassObject** caller_object, RVM_Function** caller_function, RVM_Closure** caller_closure,
                                RVM_ClassObject* callee_object, RVM_Function* callee_function, RVM_Closure* callee_closure,
                                unsigned int argument_list_size);
void           init_coroutine_entry_func_local_variable(Ring_VirtualMachine* rvm,
                                                        RingCoroutine*       co,
                                                        RVM_ClassObject*     callee_object,
                                                        RVM_Function*        callee_function,
                                                        unsigned int         argument_list_size);

int            resume_coroutine(Ring_VirtualMachine* rvm,
                                CO_ID                target_co_id,
                                RVM_ClassObject** caller_object, RVM_Function** caller_function,
                                RVM_ClassObject* callee_object, RVM_Function* callee_function);
int            yield_coroutine(Ring_VirtualMachine* rvm);
int            finish_coroutine(Ring_VirtualMachine* rvm,
                                RVM_ClassObject** caller_object, RVM_Function** caller_function,
                                RVM_Function* callee_function);

// --------------------


/* --------------------
 * thread_pool.c
 * function definition
 *
 */
typedef struct thpool_* threadpool;
threadpool              thpool_init(int num_threads);
int                     thpool_add_work(threadpool, void (*function_p)(void*), void* arg_p);
void                    thpool_wait(threadpool);
void                    thpool_pause(threadpool);
void                    thpool_resume(threadpool);
void                    thpool_destroy(threadpool);
int                     thpool_num_threads_working(threadpool);
// --------------------


#endif // RING_INCLUDE_H
