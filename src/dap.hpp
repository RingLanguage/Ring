#ifndef RING_DAP_INCLUDE_H
#define RING_DAP_INCLUDE_H

#include "json.hpp"
#include <cstdio>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

using json = nlohmann::json;

// ------- 常量和枚举定义 --------
namespace dap {

using MessageType                                                 = std::string;
const MessageType MessageType_Request                             = "request";
const MessageType MessageType_Response                            = "response";
const MessageType MessageType_Event                               = "event";

using CommandType                                                 = std::string;
const CommandType Command_Initialize                              = "initialize";
const CommandType Command_Launch                                  = "launch";
const CommandType Command_Attach                                  = "attach";
const CommandType Command_Disconnect                              = "disconnect";
const CommandType Command_Terminate                               = "terminate";
const CommandType Command_Restart                                 = "restart";
const CommandType Command_SetBreakpoints                          = "setBreakpoints";
const CommandType Command_SetFunctionBreakpoints                  = "setFunctionBreakpoints";
const CommandType Command_SetExceptionBreakpoints                 = "setExceptionBreakpoints";
const CommandType Command_ConfigurationDone                       = "configurationDone";
const CommandType Command_Continue                                = "continue";
const CommandType Command_Next                                    = "next";
const CommandType Command_StepIn                                  = "stepIn";
const CommandType Command_StepOut                                 = "stepOut";
const CommandType Command_StepBack                                = "stepBack";
const CommandType Command_ReverseContinue                         = "reverseContinue";
const CommandType Command_RestartFrame                            = "restartFrame";
const CommandType Command_Goto                                    = "goto";
const CommandType Command_Pause                                   = "pause";
const CommandType Command_StackTrace                              = "stackTrace";
const CommandType Command_Scopes                                  = "scopes";
const CommandType Command_Variables                               = "variables";
const CommandType Command_SetVariable                             = "setVariable";
const CommandType Command_Source                                  = "source";
const CommandType Command_Threads                                 = "threads";
const CommandType Command_TerminateThreads                        = "terminateThreads";
const CommandType Command_Modules                                 = "modules";
const CommandType Command_LoadedSources                           = "loadedSources";
const CommandType Command_Evaluate                                = "evaluate";
const CommandType Command_SetExpression                           = "setExpression";
const CommandType Command_StepInTargets                           = "stepInTargets";
const CommandType Command_GotoTargets                             = "gotoTargets";
const CommandType Command_Completions                             = "completions";
const CommandType Command_ExceptionInfo                           = "exceptionInfo";
const CommandType Command_ReadMemory                              = "readMemory";
const CommandType Command_WriteMemory                             = "writeMemory";
const CommandType Command_Disassemble                             = "disassemble";
const CommandType Command_Cancel                                  = "cancel";
const CommandType Command_BreakpointLocations                     = "breakpointLocations";

using EventType                                                   = std::string;
const EventType EventType_Initialized                             = "initialized";
const EventType EventType_Stopped                                 = "stopped";
const EventType EventType_Continued                               = "continued";
const EventType EventType_Exited                                  = "exited";
const EventType EventType_Terminated                              = "terminated";
const EventType EventType_Thread                                  = "thread";
const EventType EventType_Output                                  = "output";
const EventType EventType_Breakpoint                              = "breakpoint";
const EventType EventType_Module                                  = "module";
const EventType EventType_LoadedSource                            = "loadedSource";
const EventType EventType_Process                                 = "process";
const EventType EventType_Capabilities                            = "capabilities";
const EventType EventType_ProgressStart                           = "progressStart";
const EventType EventType_ProgressUpdate                          = "progressUpdate";
const EventType EventType_ProgressEnd                             = "progressEnd";

using StoppedEventReason                                          = std::string;
const StoppedEventReason StoppedEventReason_Step                  = "step";
const StoppedEventReason StoppedEventReason_Breakpoint            = "breakpoint";
const StoppedEventReason StoppedEventReason_Exception             = "exception";
const StoppedEventReason StoppedEventReason_Pause                 = "pause";
const StoppedEventReason StoppedEventReason_Entry                 = "entry";
const StoppedEventReason StoppedEventReason_Goto                  = "goto";
const StoppedEventReason StoppedEventReason_FunctionBreakpoint    = "functionBreakpoint";
const StoppedEventReason StoppedEventReason_DataBreakpoint        = "dataBreakpoint";
const StoppedEventReason StoppedEventReason_InstructionBreakpoint = "instructionBreakpoint";

enum class ErrorCode {
    ParseError       = -32700,
    InvalidRequest   = -32600,
    MethodNotFound   = -32601,
    InvalidParams    = -32602,
    InternalError    = -32603,
    ServerErrorStart = -32099,
    ServerErrorEnd   = -32000,
    RequestCanceled  = -32800,
    ContentTooLarge  = -32801,
    UnknownError     = -1
};


} // namespace dap


// ------- 基础消息类型定义 --------
namespace dap {

// 公共消息头
struct DAPMessage {
    int         seq;
    MessageType type;
    CommandType command;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DAPMessage, seq, type, command);
};

// 通用响应结构
struct DAPResponse {
    int                        seq;
    int                        request_seq;
    MessageType                type = MessageType_Response;
    CommandType                command;
    bool                       success;
    std::optional<std::string> message;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DAPResponse, seq, request_seq, type, command, success, message);
};

// Source 类型
struct Source {
    std::string                name;
    std::optional<std::string> path;
    std::optional<int>         sourceReference;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Source, name, path, sourceReference);
};

// StackFrame 类型
struct StackFrame {
    int                        id;
    std::string                name;
    std::optional<Source>      source;
    int                        line;
    int                        column;
    std::optional<int>         endLine;
    std::optional<int>         endColumn;
    std::optional<std::string> moduleId;
    std::optional<std::string> presentationHint;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StackFrame,
                                   id, name, source, line, column,
                                   endLine, endColumn, moduleId, presentationHint);
};

// Thread 类型
struct Thread {
    int                        id;
    std::string                name;
    std::optional<std::string> state;       // TODO: 使用枚举类型代替
    std::optional<std::string> pauseReason; // TODO: 使用枚举类型代替
};
inline void to_json(nlohmann::json& j, const Thread& t) {
    j = nlohmann::json{
        {"id", t.id},
        {"name", t.name},
        {"state", t.state ? nlohmann::json(*t.state) : nlohmann::json()},
        {"pauseReason", t.pauseReason ? nlohmann::json(*t.pauseReason) : nlohmann::json()}};
}
inline void from_json(const nlohmann::json& j, Thread& t) {
    j.at("id").get_to(t.id);
    j.at("name").get_to(t.name);

    if (j.contains("state") && !j["state"].is_null()) {
        t.state = j["state"].get<std::string>();
    } else {
        t.state = std::nullopt;
    }

    if (j.contains("pauseReason") && !j["pauseReason"].is_null()) {
        t.pauseReason = j["pauseReason"].get<std::string>();
    } else {
        t.pauseReason = std::nullopt;
    }
}

} // namespace dap


// ------- request/response 定义 --------
// initialize
namespace dap {
struct InitializeRequestArguments {
    std::string clientID;
    std::string clientName;
    std::string adapterID;
    std::string locale;
    bool        linesStartAt1;
    bool        columnsStartAt1;
    std::string pathFormat;

    // 能力标志 (部分示例)
    bool supportsVariableType;
    bool supportsVariablePaging;
    bool supportsRunInTerminalRequest;
    bool supportsMemoryReferences;
    // ... 其他能力标志

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        InitializeRequestArguments,
        clientID, clientName, adapterID, locale,
        linesStartAt1, columnsStartAt1, pathFormat,
        supportsVariableType, supportsVariablePaging,
        supportsRunInTerminalRequest, supportsMemoryReferences)
};
// 异常断点过滤器
struct ExceptionBreakpointsFilter {
    std::string filter;
    std::string label;
    bool        default_;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        ExceptionBreakpointsFilter,
        filter, label, default_)
};

// Initialize 响应体
struct InitializeResponseBody {
    bool                                    supportsConfigurationDoneRequest;
    bool                                    supportsFunctionBreakpoints;
    bool                                    supportsConditionalBreakpoints;
    std::vector<ExceptionBreakpointsFilter> exceptionBreakpointFilters;
    bool                                    supportsSetVariable;
    bool                                    supportsStepBack;
    // ... 其他能力标志

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        InitializeResponseBody,
        supportsConfigurationDoneRequest,
        supportsFunctionBreakpoints,
        supportsConditionalBreakpoints,
        exceptionBreakpointFilters,
        supportsSetVariable,
        supportsStepBack)
};


struct InitializeRequest : DAPMessage {
    InitializeRequestArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitializeRequest, seq, type, command, arguments);
};
struct InitializeResponse : DAPResponse {
    InitializeResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitializeResponse, seq, request_seq, type, command, success, message, body);
};

} // namespace dap


// ------- request/response 定义 --------
// launch
namespace dap {

struct LaunchRequestArguments {
    std::string                                      program;
    std::vector<std::string>                         args;
    std::string                                      cwd;
    std::vector<std::pair<std::string, std::string>> environment;
    bool                                             stopAtEntry;
    std::string                                      console;
    std::optional<std::string>                       symbolSearchPath;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        LaunchRequestArguments,
        program, args, cwd, environment,
        stopAtEntry, console, symbolSearchPath)
};
// 错误信息
struct LaunchErrorBody {
    int                                id;
    std::string                        format;
    std::map<std::string, std::string> variables;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        LaunchErrorBody,
        id, format, variables)
};

struct LaunchResponseBody {
    std::optional<LaunchErrorBody> error; // 可选的错误信息
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LaunchResponseBody, error);
};

struct LaunchRequest : DAPMessage {
    LaunchRequestArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LaunchRequest, seq, type, command, arguments);
};
struct LaunchResponse : DAPResponse {
    LaunchResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LaunchResponse, seq, request_seq, type, command, success, message, body);
};


} // namespace dap


// ------- request/response 定义 --------
// threads
namespace dap {


// threads 请求 (无参数)
struct ThreadsRequest : DAPMessage {};

// threads 响应
struct ThreadsResponseBody {
    std::vector<Thread> threads;
};
struct ThreadsResponse : DAPResponse {
    ThreadsResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ThreadsResponse, seq, request_seq, type, command, success, message, body);
};

// JSON 序列化/反序列化
inline void to_json(json& j, const ThreadsResponseBody& t) {
    j = json{{"threads", t.threads}};
}

inline void from_json(const json& j, ThreadsResponseBody& t) {
    j.at("threads").get_to(t.threads);
}


} // namespace dap


// ------- request/response 定义 --------
// stackTrace
namespace dap {

// stackTrace 请求参数
struct StackTraceArguments {
    int threadId;
    int startFrame;
    int levels;
};
struct StackTraceRequest : DAPMessage {
    StackTraceArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StackTraceRequest, seq, type, command, arguments);
};

struct StackTraceResponseBody {
    std::vector<StackFrame> stackFrames;
    std::optional<int>      totalFrames;
};
struct StackTraceResponse : DAPResponse {
    StackTraceResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StackTraceResponse, seq, request_seq, type, command, success, message, body);
};


inline void to_json(json& j, const StackTraceArguments& s) {
    j = json{{"threadId", s.threadId}};
    if (s.startFrame)
        j["startFrame"] = s.startFrame;
    if (s.levels)
        j["levels"] = s.levels;
}
inline void from_json(const json& j, StackTraceArguments& s) {
    j.at("threadId").get_to(s.threadId);
    if (j.contains("startFrame"))
        s.startFrame = j["startFrame"];
    if (j.contains("levels"))
        s.levels = j["levels"];
}

inline void to_json(json& j, const StackTraceResponseBody& s) {
    j = json{{"stackFrames", s.stackFrames}};
    if (s.totalFrames)
        j["totalFrames"] = *s.totalFrames;
}
inline void from_json(const json& j, StackTraceResponseBody& s) {
    j.at("stackFrames").get_to(s.stackFrames);
    if (j.contains("totalFrames"))
        s.totalFrames = j["totalFrames"];
}

} // namespace dap

// ------- request/response 定义 --------
// scopes variables
namespace dap {
struct ScopesArguments {
    int frameId; // 堆栈帧 ID
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScopesArguments, frameId);
};

struct Scope {
    std::string           name;               // 作用域名称（如 "Local"、"Global"）
    int                   variablesReference; // 用于后续获取变量（variables 请求）
    bool                  expensive;          // 是否计算成本高
    std::optional<Source> source;             // 可选，关联的源代码
};
inline void to_json(json& j, const Scope& c) {
    j = json{
        {"name", c.name},
        {"variablesReference", c.variablesReference},
        {"expensive", c.expensive},
    };
    if (c.source)
        j["source"] = *c.source;
}

inline void from_json(const json& j, Scope& c) {
    j.at("name").get_to(c.name);
    j.at("variablesReference").get_to(c.variablesReference);
    j.at("expensive").get_to(c.expensive);
    if (j.contains("source"))
        c.source = j["source"];
}

struct ScopesResponseseBody {
    std::vector<Scope> scopes; // 作用域列表
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScopesResponseseBody, scopes);
};


struct ScopesRequest : DAPMessage {
    ScopesArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScopesRequest, seq, type, command, arguments);
};
struct ScopesResponse : DAPResponse {
    ScopesResponseseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScopesResponse, seq, request_seq, type, command, success, message, body);
};
} // namespace dap


// ------- request/response 定义 --------
// variables
namespace dap {
struct VariablesRequestArguments {
    int                        variablesReference; // 变量引用（来自 scopes/previous variables 响应）
    std::optional<std::string> filter;             // 可选："named"（默认）、"indexed"（数组/字典索引）、"all"
    std::optional<int>         start;              // 可选：分页起始索引
    std::optional<int>         count;              // 可选：返回的变量数量
};
inline void to_json(json& j, const VariablesRequestArguments& c) {
    j = json{
        {"variablesReference", c.variablesReference},
    };
    if (c.filter)
        j["filter"] = *c.filter;
    if (c.start)
        j["start"] = *c.start;
    if (c.count)
        j["count"] = *c.count;
}

inline void from_json(const json& j, VariablesRequestArguments& c) {
    j.at("variablesReference").get_to(c.variablesReference);
    if (j.contains("filter"))
        c.filter = j["filter"];
    if (j.contains("start"))
        c.start = j["start"];
    if (j.contains("count"))
        c.count = j["count"];
}


struct Variable {
    std::string                name;               // 变量名（如 "count"）
    std::string                value;              // 变量值的字符串表示（如 "42"）
    std::optional<std::string> type;               // 可选：变量类型（如 "int"）
    int                        variablesReference; // 非 0 表示可进一步展开（子变量）
    std::optional<std::string> evaluateName;       // 可选：用于表达式求值（如 "array[0]"）
    std::optional<int>         indexedVariables;   // 可选：数组/字典的元素数量
    std::optional<int>         namedVariables;     // 可选：具名子变量数量
};
inline void to_json(json& j, const Variable& c) {
    j = json{
        {"name", c.name},
        {"value", c.value},
        {"variablesReference", c.variablesReference},
    };
    if (c.type)
        j["type"] = *c.type;
    if (c.evaluateName)
        j["evaluateName"] = *c.evaluateName;
    if (c.indexedVariables)
        j["indexedVariables"] = *c.indexedVariables;
    if (c.namedVariables)
        j["namedVariables"] = *c.namedVariables;
}
inline void from_json(const json& j, Variable& c) {
    j.at("name").get_to(c.name);
    j.at("value").get_to(c.value);
    j.at("variablesReference").get_to(c.variablesReference);
    if (j.contains("type"))
        c.type = j["type"];
    if (j.contains("evaluateName"))
        c.evaluateName = j["evaluateName"];
    if (j.contains("indexedVariables"))
        c.indexedVariables = j["indexedVariables"];
    if (j.contains("namedVariables"))
        c.namedVariables = j["namedVariables"];
}
struct VariablesResponseBody {
    std::vector<Variable> variables; // 变量列表
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(VariablesResponseBody, variables);
};

struct VariablesRequest : DAPMessage {
    VariablesRequestArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(VariablesRequest, seq, type, command, arguments);
};
struct VariablesResponse : DAPResponse {
    VariablesResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(VariablesResponse, seq, request_seq, type, command, success, message, body);
};
} // namespace dap


// ------- request/response 定义 --------
// continue
namespace dap {

struct ContinueArguments {
    int                 threadId;
    std::optional<bool> singleThread;
};

struct ContinueRequest : DAPMessage {
    ContinueArguments arguments;
};

struct ContinueResponseBody {
    std::optional<bool> allThreadsContinued;
};
struct ContinueResponse : DAPResponse {
    ContinueResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ContinueResponse, seq, request_seq, type, command, success, message, body);
};

inline void to_json(json& j, const ContinueArguments& c) {
    j = json{{"threadId", c.threadId}};
    if (c.singleThread)
        j["singleThread"] = *c.singleThread;
}

inline void from_json(const json& j, ContinueArguments& c) {
    j.at("threadId").get_to(c.threadId);
    if (j.contains("singleThread"))
        c.singleThread = j["singleThread"];
}

inline void to_json(json& j, const ContinueResponseBody& c) {
    if (c.allThreadsContinued)
        j["allThreadsContinued"] = *c.allThreadsContinued;
}

inline void from_json(const json& j, ContinueResponseBody& c) {
    if (j.contains("allThreadsContinued")) {
        c.allThreadsContinued = j["allThreadsContinued"];
    }
}

} // namespace dap


// ------- request/response 定义 --------
// next/stepIn/stepOut
namespace dap {

struct StepArguments {
    int         threadId;
    bool        singleThread;
    std::string granularity; // "statement", "line", or "instruction" TODO: 抽象出 const
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepArguments, threadId, singleThread, granularity);
};
struct StepResponseBody {
    bool allThreadsContinued;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepResponseBody, allThreadsContinued);
};

struct NextRequest : DAPMessage {
    StepArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NextRequest, seq, type, command, arguments);
};
struct StepInRequest : DAPMessage {
    StepArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepInRequest, seq, type, command, arguments);
};
struct StepOutRequest : DAPMessage {
    StepArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepOutRequest, seq, type, command, arguments);
};


struct NextResponse : DAPResponse {
    StepResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NextResponse, seq, request_seq, type, command, success, message, body);
};
struct StepInResponse : DAPResponse {
    StepResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepInResponse, seq, request_seq, type, command, success, message, body);
};
struct StepOutResponse : DAPResponse {
    StepResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StepOutResponse, seq, request_seq, type, command, success, message, body);
};

} // namespace dap


// ------- request/response 定义 --------
// setBreakPointsRequest
namespace dap {

struct Breakpoint {
    int                        line;         // 行号（从1开始）
    std::optional<std::string> condition;    // 条件表达式（如 "x > 5"）
    std::optional<std::string> hitCondition; // 命中条件（如 ">=3"）
};

inline void to_json(json& j, const Breakpoint& c) {
    j = json{{"line", c.line}};
    if (c.condition)
        j["condition"] = *c.condition;
    if (c.hitCondition)
        j["condition"] = *c.hitCondition;
}

inline void from_json(const json& j, Breakpoint& c) {
    j.at("line").get_to(c.line);
    if (j.contains("condition"))
        c.condition = j["condition"];
    if (j.contains("hitCondition"))
        c.hitCondition = j["hitCondition"];
}

struct SetBreakpointsArguments {
    Source                  source;         // 源代码文件
    std::vector<Breakpoint> breakpoints;    // 断点列表
    std::optional<bool>     sourceModified; // 可选（源码是否被修改）
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetBreakpointsArguments, source, breakpoints, sourceModified);
};

struct BreakpointResponseInfo {
    int         id;       // 断点唯一标识符
    int         line;     // 实际设置的行号
    bool        verified; // 是否成功设置
    std::string message;  // 状态信息（如错误原因）
    Source      source;   // 关联的源代码
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BreakpointResponseInfo, id, line, verified, message, source);
};

// 响应体（SetBreakpointsResponseBody）
struct SetBreakpointsResponseBody {
    std::vector<BreakpointResponseInfo> breakpoints; // 断点设置结果
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetBreakpointsResponseBody, breakpoints);
};

struct SetBreakpointsRequest : DAPMessage {
    SetBreakpointsArguments arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetBreakpointsRequest, seq, type, command, arguments);
};

struct SetBreakpointsResponse : DAPResponse {
    SetBreakpointsResponseBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SetBreakpointsResponse, seq, request_seq, type, command, success, message, body);
};

} // namespace dap

// ------- event 定义 --------
namespace dap {

struct InitializedEvent {
    int         seq   = 0;
    MessageType type  = MessageType_Event;
    EventType   event = EventType_Initialized;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitializedEvent, seq, type, event);
};

struct StoppedEvent {
    int         seq   = 0;
    MessageType type  = MessageType_Event;
    EventType   event = EventType_Stopped;
    struct Body {
        StoppedEventReason reason;            // 停止的原因
        int                threadId;          // 停止的线程ID
        bool               allThreadsStopped; // 是否所有线程都已停止
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Body, reason, threadId, allThreadsStopped);
    } body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StoppedEvent, seq, type, event, body);
};


struct OutputEventBody {
    std::string output;                 // 输出内容
    std::string category;               // 输出类别
    int         variablesReference = 0; // 变量引用
    std::string source;
    int         line;
    int         column;
    std::string data;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OutputEventBody, output, category, variablesReference, source, line, column, data);
};
struct OutputEvent {
    enum class Category {
        Console,
        Important,
        Stdout,
        Stderr,
        Telemetry
    };
    int             seq   = 0;
    MessageType     type  = MessageType_Event;
    EventType       event = EventType_Output;
    OutputEventBody body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OutputEvent, seq, type, event, body);
};


} // namespace dap


#endif // RING_DAP_INCLUDE_H
