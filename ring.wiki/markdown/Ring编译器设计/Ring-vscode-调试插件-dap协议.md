

## stoppedEvent


```json
{
  "type": "event",
  "event": "stopped",
  "body": {
    "reason": "entry",
    "threadId": 1,
    "allThreadsStopped": true,
    "preserveFocusHint": false,
    "text": "Paused at entry point",
    "description": "Program paused at the entry point before execution begins"
  },
  "seq": 123
}
```

type: 固定为 "event"，表示这是一个事件
event: 固定为 "stopped"，表示线程停止事件
body.reason: 值为 "entry"，表示停止原因是程序入口点
  - entry
  - step  stepIn/stepOut/step 指令
body.threadId: 停止的线程ID
body.allThreadsStopped: 布尔值，表示是否所有线程都停止了
body.preserveFocusHint: 可选，提示UI是否应保持当前焦点
body.text: 可选的简短描述文本
body.description: 可选的详细描述文本
seq: 序列号，用于协议通信的顺序标识

## terminatedEvent

```json
{
  "event": "terminated",
  "body": {
    "restart": false
  },
  "type": "event",
  "seq": 123
}
```


## exitedEvent

```json
{
  "event": "exited",
  "body": {
    "exitCode": 0
  },
  "type": "event",
  "seq": 124
}
```



## initialize 的 request/response


Initialize 请求 (Request)
客户端发送给调试器的初始化请求：

```json
{
  "type": "request",
  "command": "initialize",
  "seq": 1,
  "arguments": {
    "clientID": "vscode",
    "clientName": "Visual Studio Code",
    "adapterID": "cppdbg",
    "locale": "en-US",
    "linesStartAt1": true,
    "columnsStartAt1": true,
    "pathFormat": "path",
    "supportsVariableType": true,
    "supportsVariablePaging": true,
    "supportsRunInTerminalRequest": true,
    "supportsMemoryReferences": true,
    "supportsProgressReporting": true,
    "supportsInvalidatedEvent": true,
    "supportsMemoryEvent": true,
    "supportsArgsCanBeInterpretedByShell": true
  }
}
```

关键参数说明:

参数	类型	说明
clientID	string	客户端标识符
clientName	string	客户端显示名称
adapterID	string	调试器适配器类型
locale	string	客户端区域设置
linesStartAt1	boolean	是否使用1-based行号
columnsStartAt1	boolean	是否使用1-based列号
pathFormat	string	路径格式 ("path" 或 "uri")
supports*	boolean	各种能力标志



Initialize 响应 (Response)
调试器返回的初始化响应：


```json
{
  "type": "response",
  "command": "initialize",
  "success": true,
  "seq": 1,
  "request_seq": 1,
  "body": {
    "supportsConfigurationDoneRequest": true,
    "supportsFunctionBreakpoints": true,
    "supportsConditionalBreakpoints": true,
    "supportsHitConditionalBreakpoints": true,
    "supportsEvaluateForHovers": true,
    "exceptionBreakpointFilters": [
      {
        "filter": "cpp_exceptions",
        "label": "C++ Exceptions",
        "default": false
      }
    ],
    "supportsStepBack": false,
    "supportsSetVariable": true,
    "supportsRestartFrame": true,
    "supportsGotoTargetsRequest": true,
    "supportsStepInTargetsRequest": true,
    "supportsCompletionsRequest": true,
    "supportsModulesRequest": true,
    "additionalModuleColumns": [],
    "supportedChecksumAlgorithms": ["MD5", "SHA1"],
    "supportsRestartRequest": true,
    "supportsExceptionOptions": true,
    "supportsValueFormattingOptions": true,
    "supportsExceptionInfoRequest": true,
    "supportTerminateDebuggee": true,
    "supportsDelayedStackTraceLoading": true,
    "supportsLoadedSourcesRequest": true,
    "supportsLogPoints": true,
    "supportsTerminateThreadsRequest": true,
    "supportsSetExpression": true,
    "supportsTerminateRequest": true,
    "supportsDataBreakpoints": true,
    "supportsReadMemoryRequest": true,
    "supportsDisassembleRequest": true,
    "supportsCancelRequest": true,
    "supportsBreakpointLocationsRequest": true
  }
}

```



## Initialized event


```json
{
  "type": "event",
  "event": "initialized",
  "seq": 2,
  "body": {}
}
``` 


## launch request/response

1. launch 请求 (Request)
客户端发送给调试器的请求：

```json
{
  "type": "request",
  "command": "launch",
  "seq": 4,
  "arguments": {
    "program": "/path/to/executable",
    "args": ["arg1", "arg2"],
    "cwd": "/working/directory",
    "environment": [{"name": "ENV_VAR", "value": "value"}],
    "stopAtEntry": true,
    "console": "externalTerminal",
    "symbolSearchPath": "/path/to/symbols"
  }
}
```

常见参数:

program: 要调试的可执行文件路径
args: 程序参数数组
cwd: 工作目录
environment: 环境变量设置
stopAtEntry: 是否在入口点暂停
console: 控制台类型 ("internalConsole", "integratedTerminal", "externalTerminal")
symbolSearchPath: 符号文件搜索路径



2. launch 响应 (Response)
调试器返回的响应：

成功响应：

```json
{
  "type": "response",
  "command": "launch",
  "success": true,
  "seq": 4,
  "request_seq": 4,
  "body": {}
}
```


失败响应：

```json
{
  "type": "response",
  "command": "launch",
  "success": false,
  "seq": 4,
  "request_seq": 4,
  "message": "Failed to launch: executable not found",
  "body": {
    "error": {
      "id": 3000,
      "format": "Unable to start debugging. Program '{path}' not found.",
      "variables": {
        "path": "/path/to/executable"
      }
    }
  }
}
```



## threads 的 request和response


Threads 请求 (Request)
客户端发送给调试器的请求格式：

```json
{
  "type": "request",
  "command": "threads",
  "arguments": {},
  "seq": 123
}
```

请求说明:
这是最简单的 DAP 请求之一，通常不需要任何参数
当调试会话开始或线程状态改变时，客户端通常会发送此请求


Threads 响应 (Response)
调试器返回的响应格式：

```json
{
  "type": "response",
  "command": "threads",
  "success": true,
  "body": {
    "threads": [
      {
        "id": 1,
        "name": "main thread",
        "state": "stopped",
        "pauseReason": "entry"
      },
      {
        "id": 2,
        "name": "worker thread #1",
        "state": "running"
      },
      {
        "id": 3,
        "name": "background thread",
        "state": "stopped",
        "pauseReason": "breakpoint"
      }
    ]
  },
  "seq": 123,
  "request_seq": 123
}
```

响应参数说明:
threads 数组中的每个线程对象包含：
id (必需): 线程的唯一标识符
name (必需): 线程的名称(用于显示)
state (可选): 线程状态，通常是：
"running": 线程正在运行
"stopped": 线程已停止
"terminated": 线程已终止

pauseReason (可选): 如果线程停止，说明停止原因，常见值包括：
"entry": 在程序入口点停止
"breakpoint": 因断点停止
"exception": 因异常停止
"pause": 用户手动暂停
"step": 单步执行后停止
"debugger_stopped": 调试器停止



如果请求失败，响应可能如下：

```json
{
  "type": "response",
  "command": "threads",
  "success": false,
  "message": "Debug session not active",
  "seq": 123,
  "request_seq": 123
}
```


使用场景
调试会话初始化：当调试会话启动时，客户端通常会请求线程列表
线程状态变化：当收到stopped事件时，客户端可能会刷新线程列表
多线程调试：在多线程程序中，客户端需要定期更新线程状态

注意事项
线程ID在整个调试会话期间必须保持稳定
即使没有多线程，调试器也应返回至少一个线程(通常是主线程)
线程名称应尽可能具有描述性，便于用户识别




## stacktrace 的 request 和 response


StackTrace 请求 (Request)
客户端发送给调试器的请求格式：

```json
{
  "type": "request",
  "command": "stackTrace",
  "arguments": {
    "threadId": 1,
    "startFrame": 0,
    "levels": 20,
    "format": {
      "parameters": true,
      "parameterTypes": true,
      "parameterNames": true,
      "line": true,
      "module": true,
      "includeAll": false
    }
  },
  "seq": 42
}
```

请求参数说明:
threadId (必需): 要获取堆栈的线程ID
startFrame (可选): 从哪个帧开始获取(默认为0，即最顶层的帧)
levels (可选): 要获取的帧数量(默认为所有帧)
format (可选): 控制堆栈帧如何格式化
parameters: 是否包含参数值
parameterTypes: 是否包含参数类型
parameterNames: 是否包含参数名
line: 是否包含源代码行
module: 是否包含模块名
includeAll: 是否包含所有可能的信息



StackTrace 响应 (Response)
调试器返回的响应格式：


```json
{
  "type": "response",
  "command": "stackTrace",
  "success": true,
  "body": {
    "stackFrames": [
      {
        "id": 1000,
        "name": "main",
        "source": {
          "name": "app.js",
          "path": "/path/to/app.js",
          "sourceReference": 0
        },
        "line": 10,
        "column": 5,
        "endLine": 10,
        "endColumn": 15,
        "moduleId": "module1",
        "presentationHint": "normal"
      },
      {
        "id": 1001,
        "name": "calculate",
        "source": {
          "name": "math.js",
          "path": "/path/to/math.js",
          "sourceReference": 0
        },
        "line": 25,
        "column": 10,
        "endLine": 25,
        "endColumn": 20,
        "moduleId": "module2",
        "presentationHint": "label"
      }
    ],
    "totalFrames": 15
  },
  "seq": 42,
  "request_seq": 42
}
```

响应参数说明:
stackFrames (必需): 堆栈帧数组，按从顶部到底部的顺序排列
id: 帧的唯一标识符(用于后续的scopes请求)
name: 帧的名称(通常是函数名)
source: 源代码位置信息
line, column: 当前执行位置
endLine, endColumn: 可选，函数/块的结束位置
moduleId: 可选，模块标识符
presentationHint: 可选，显示提示("normal", "label", "subtle")
totalFrames (可选): 总帧数(如果levels参数限制了返回数量)

如果请求失败，响应可能如下：

```json
{
  "type": "response",
  "command": "stackTrace",
  "success": false,
  "message": "Thread not found",
  "seq": 42,
  "request_seq": 42
}
```


## scopes request/response


```json
{
  "command": "scopes",
  "arguments": {
    "frameId": 123  // 堆栈帧的唯一标识符（来自 stackTrace 响应）
  },
  "seq": 42,        // 请求序列号
  "type": "request"
}
```




字段说明
frameId：来自 stackTrace 响应的堆栈帧 ID，表示要查询哪个帧的作用域。


```json
{
  "command": "scopes",
  "success": true,
  "body": {
    "scopes": [
      {
        "name": "Local",
        "variablesReference": 1001,  // 用于后续 variables 请求
        "expensive": false,          // 是否计算成本高（如全局作用域可能较慢）
        "source": {                  // 可选，作用域关联的源代码
          "path": "/path/to/file.cpp"
        }
      },
      {
        "name": "Global",
        "variablesReference": 1002,
        "expensive": true
      }
    ]
  },
  "seq": 42,       // 对应请求的序列号
  "type": "response"
}
```

字段说明
scopes：返回的作用域列表，每个作用域包含：
name：作用域名称（如 "Local"、"Global"）。
variablesReference：用于后续 variables 请求获取该作用域的变量（如果为 0 表示无变量）。
expensive：如果为 true，表示获取该作用域的变量可能较慢（如全局作用域）。
source（可选）：作用域关联的源代码位置。


3. 典型工作流程

获取 threads → 返回 threadId。

获取堆栈帧（stackTrace 请求）→ 返回 frameId。

获取作用域（scopes 请求）→ 返回作用域列表（如 Local、Global）。

获取变量（variables 请求）→ 使用 variablesReference 查询具体变量值。



## variables 的 request/response

variables 请求用于获取某个作用域（scope）或变量引用（variable reference）中的变量列表



```json
{
  "command": "variables",
  "arguments": {
    "variablesReference": 1001,  // 来自 scopes/variables 响应的引用
    "filter": "named",           // 可选：过滤类型（"named" | "indexed" | "all"）
    "start": 0,                  // 可选：分页起始索引
    "count": 100                 // 可选：返回的变量数量
  },
  "seq": 42,                     // 请求序列号
  "type": "request"
}
```



字段说明
variablesReference：来自 scopes 或 variables 响应的引用 ID。
filter（可选）：
"named"：仅返回具名变量（默认）。
"indexed"：仅返回数组/字典的索引变量（如 [0], [1]）。
"all"：返回所有变量。
start 和 count（可选）：用于分页加载大量变量。



```json
{
  "command": "variables",
  "success": true,
  "body": {
    "variables": [
      {
        "name": "count",
        "value": "42",
        "type": "int",
        "variablesReference": 0,      // 0 表示无子变量
        "evaluateName": "count"       // 可选：用于表达式求值的名称
      },
      {
        "name": "array",
        "value": "int[3]",
        "type": "array",
        "variablesReference": 1002,   // 非 0 表示可进一步展开
        "indexedVariables": 3        // 可选：数组/字典的元素数量
      }
    ]
  },
  "seq": 42,                        // 对应请求的序列号
  "type": "response"
}
```



字段说明
variables：变量列表，每个变量包含：
name 和 value：变量的名称和值的字符串表示。
type（可选）：变量类型（如 "int"、"string"）。

variablesReference：
0：表示该变量无子变量。
非 0：可通过新的 variables 请求展开（如数组、结构体）。

evaluateName（可选）：用于表达式求值的名称（如 "array[0]"）。
indexedVariables/namedVariables（可选）：提示客户端可展开的子变量数量。


3. 典型工作流程
获取作用域（scopes 请求）→ 返回 variablesReference（如 1001）。

获取变量（variables 请求）→ 使用 variablesReference 查询变量列表。

展开复杂变量（如数组）→ 对 variablesReference != 0 的变量发起新的 variables 请求。



## continue 指令 的 request/response


Continue 请求 (Request)
客户端发送给调试器的请求格式：

```json
{
  "type": "request",
  "command": "continue",
  "arguments": {
    "threadId": 1,
    "singleThread": false
  },
  "seq": 456
}
```

请求参数说明:

threadId (必需): 要恢复执行的线程ID
singleThread (可选): 布尔值，表示是否只恢复指定线程(在多线程调试中)
  如果为 true，则只恢复指定线程
  如果为 false 或省略，则恢复所有暂停的线程


Continue 响应 (Response)
调试器返回的成功响应格式：

```json
{
  "type": "response",
  "command": "continue",
  "success": true,
  "body": {
    "allThreadsContinued": false
  },
  "seq": 456,
  "request_seq": 456
}
```

响应参数说明:

body.allThreadsContinued (可选): 布尔值，表示是否所有线程都被恢复了
如果为 true，表示所有线程都恢复了执行
如果为 false，表示只有请求中指定的线程恢复了执行


如果请求失败，响应可能如下：

```json
{
  "type": "response",
  "command": "continue",
  "success": false,
  "message": "Thread not found",
  "seq": 456,
  "request_seq": 456
}
```


## next 指令（stepOver）


```json
{
  "command": "next",
  "arguments": {
    "threadId": 1,
    "singleThread": false,
    "granularity": "statement"
  },
  "type": "request",
  "seq": 125
}
```


```json
{
  "type": "response",
  "request_seq": 125,
  "success": true,
  "command": "next",
  "body": {},
  "seq": 459
}
```

## stepIn 指令




```json
{
  "type": "request",
  "command": "stepIn",
  "arguments": {
    "threadId": 1,
    "singleThread": false,
    "granularity": "statement"
  },
  "seq": 123
}
```

arguments.granularity (optional): The granularity of the step ("statement", "line", or "instruction")



```json
{
  "type": "response",
  "request_seq": 123,
  "success": true,
  "command": "stepIn",
  "body": {},
  "seq": 456
}
```


## stepOut 指令

stepOut 的具体作用
当你在调试过程中进入了一个函数调用（比如使用了 stepInto 指令），stepOut 会让你：

执行完当前函数的剩余部分

返回到调用这个函数的地方

在函数返回后的第一条语句处暂停


```json
{
  "command": "stepOut",
  "arguments": {
    "threadId": 1,
    "singleThread": false,
    "granularity": "statement"
  },
  "type": "request",
  "seq": 124
}
```


```json
{
  "type": "response",
  "request_seq": 124,
  "success": true,
  "command": "stepOut",
  "body": {},
  "seq": 457
}
```


## setBreakpointsRequest


```json
{
  "command": "setBreakpoints",
  "arguments": {
    "source": {
      "path": "/path/to/file.js",
      "name": "file.js"
    },
    "breakpoints": [
      {
        "line": 10,
        "condition": "x > 5",
        "hitCondition": ">= 3"
      },
      {
        "line": 20
      }
    ],
    "lines": [10, 20],  // 可选（部分调试器可能用此替代 `breakpoints`）
    "sourceModified": false  // 可选（标记源文件是否被修改）
  },
  "seq": 42,  // 请求序列号
  "type": "request"
}
```


字段说明：
source：断点所在的源文件（必须包含 path 或 sourceReference）。
breakpoints：断点数组，每个断点可包含：
line：断点行号（从 1 开始）。
condition：条件断点的表达式（可选）。
hitCondition：命中次数条件（如 ">=3" 表示第 3 次命中时暂停，可选）。
lines：简化版断点设置（仅行号，部分调试器支持）。
sourceModified：如果为 true，调试器可能需要重新定位断点。



setBreakpoints Response

```json
{
  "command": "setBreakpoints",
  "success": true,
  "body": {
    "breakpoints": [
      {
        "id": 1,
        "line": 10,
        "verified": true,
        "message": "Breakpoint set",
        "source": {
          "path": "/path/to/file.js"
        }
      },
      {
        "id": 2,
        "line": 20,
        "verified": false,
        "message": "Line is not executable",
        "source": {
          "path": "/path/to/file.js"
        }
      }
    ]
  },
  "seq": 42,  // 对应请求的序列号
  "type": "response"
}
```


字段说明：
breakpoints：返回实际设置的断点状态数组：

id：断点唯一标识符（用于后续操作）。

verified：是否成功设置（如行号不可执行会返回 false）。

message：附加信息（例如失败原因）。

line：实际设置的行号（可能与请求不同，如自动调整到最近的可执行行）。

## exited/terminated


程序正常结束：

```json
{
  "type": "event",
  "event": "exited",
  "body": {
    "exitCode": 0
  },
  "seq": 457
}
```


程序异常终止：

```json
{
  "type": "event",
  "event": "terminated",
  "body": {},
  "seq": 457
}
```