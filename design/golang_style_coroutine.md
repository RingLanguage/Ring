# Golang 1.0 风格的协程系统设计方案

## 当前实现分析

目前Ring语言的协程实现是基于协作式的，具有以下特点：

1. 使用`RingCoroutine`结构体表示协程，包含协程ID、父协程ID、状态、运行栈和调用信息等
2. 通过`resume_coroutine`和`yield_coroutine`函数实现协程切换
3. 协程切换时保存当前协程的状态并恢复目标协程的状态
4. 协程状态保存在全局的`coroutine_map`中

## Golang 1.0 风格的 M 和 G 协程设计

### 设计目标
实现一个类似Golang 1.0的协程系统，只包含M（Machine，物理线程）和G（Goroutine，协程）两个核心组件。

### 核心概念

- **G（Goroutine）**：协程的抽象，相当于现有的`RingCoroutine`
- **M（Machine）**：物理线程，负责执行协程
- **调度器**：管理M和G，实现协程调度

### 数据结构设计

#### 1. 扩展 RingCoroutine 结构体
```cpp
struct RingCoroutine {
    CO_ID             co_id;         // 协程ID
    CO_STAT           status;        // 协程状态
    RVM_RuntimeStack* runtime_stack; // 运行栈
    RVM_CallInfo*     call_info;     // 调用信息
    
    // 新增字段
    M_ID              m_id;          // 当前执行该协程的M的ID
    uint64_t          stack_size;    // 栈大小
    uint64_t          goid;          // 协程唯一标识
    bool              is_system;     // 是否为系统协程
    
    // defer调用链
    unsigned int      defer_list_size;
    RVM_DeferItem*    defer_list;
};
```

#### 2. 添加 Machine 结构体
```cpp
typedef unsigned long long M_ID;

struct Machine {
    M_ID              m_id;          // 机器ID
    pthread_t         thread_id;     // 物理线程ID
    RingCoroutine*    running_g;     // 当前正在运行的G
    std::vector<RingCoroutine*> local_runnable_gs; // 本地可运行队列
    std::atomic<int>  park_count;    // 挂起计数
    bool              is_main;       // 是否为主线程
};
```

#### 3. 添加调度器结构体
```cpp
typedef enum {
    G_STATUS_IDLE,
    G_STATUS_RUNNABLE,
    G_STATUS_RUNNING,
    G_STATUS_SUSPENDED,
    G_STATUS_DEAD,
} G_STATUS;

struct Scheduler {
    std::vector<Machine*> machines;   // 所有机器
    std::deque<RingCoroutine*> global_runnable_gs; // 全局可运行队列
    std::atomic<int> global_runnable_count; // 全局可运行协程计数
    std::mutex       global_queue_mutex;  // 全局队列锁
};
```

### 调度器实现

#### 1. 初始化调度器
```cpp
Scheduler* init_scheduler() {
    Scheduler* sched = (Scheduler*)mem_alloc(NULL_MEM_POOL, sizeof(Scheduler));
    sched->global_runnable_count = 0;
    return sched;
}
```

#### 2. 创建机器（物理线程）
```cpp
Machine* create_machine(Scheduler* sched, bool is_main) {
    Machine* m = (Machine*)mem_alloc(NULL_MEM_POOL, sizeof(Machine));
    m->m_id = get_next_machine_id();
    m->running_g = nullptr;
    m->is_main = is_main;
    m->park_count = 0;
    
    // 将机器添加到调度器
    sched->machines.push_back(m);
    
    if (!is_main) {
        // 创建并启动物理线程
        pthread_create(&m->thread_id, NULL, machine_run, m);
    }
    
    return m;
}
```

#### 3. 机器运行函数
```cpp
void* machine_run(void* arg) {
    Machine* m = (Machine*)arg;
    Scheduler* sched = get_global_scheduler();
    
    while (true) {
        // 从本地队列获取可运行的协程
        RingCoroutine* g = nullptr;
        
        // 尝试从本地队列获取
        if (!m->local_runnable_gs.empty()) {
            g = m->local_runnable_gs.back();
            m->local_runnable_gs.pop_back();
        } else {
            // 本地队列为空，尝试从全局队列获取
            g = steal_work(sched, m);
            if (g == nullptr) {
                // 没有可运行的协程，挂起
                park_machine(m);
                continue;
            }
        }
        
        // 执行协程
        execute_g(m, g);
    }
    
    return nullptr;
}
```

#### 4. 窃取工作机制
```cpp
RingCoroutine* steal_work(Scheduler* sched, Machine* m) {
    RingCoroutine* g = nullptr;
    
    // 尝试从全局队列获取
    std::lock_guard<std::mutex> lock(sched->global_queue_mutex);
    if (!sched->global_runnable_gs.empty()) {
        g = sched->global_runnable_gs.front();
        sched->global_runnable_gs.pop_front();
        sched->global_runnable_count--;
        return g;
    }
    
    // 尝试从其他机器窃取
    for (auto other_m : sched->machines) {
        if (other_m == m) continue;
        
        std::lock_guard<std::mutex> other_lock(other_m->local_queue_mutex);
        if (other_m->local_runnable_gs.size() > 1) {
            // 窃取一半的协程
            size_t steal_count = other_m->local_runnable_gs.size() / 2;
            for (size_t i = 0; i < steal_count; i++) {
                m->local_runnable_gs.push_back(other_m->local_runnable_gs.front());
                other_m->local_runnable_gs.pop_front();
            }
            if (!m->local_runnable_gs.empty()) {
                g = m->local_runnable_gs.back();
                m->local_runnable_gs.pop_back();
                return g;
            }
        }
    }
    
    return nullptr;
}
```

#### 5. 协程执行函数
```cpp
void execute_g(Machine* m, RingCoroutine* g) {
    // 设置运行状态
    g->status = G_STATUS_RUNNING;
    m->running_g = g;
    g->m_id = m->m_id;
    
    // 记录开始运行时间
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    g->last_run_time = (long long)(ts.tv_sec) * 1000000000 + ts.tv_nsec;
    
    // 执行协程代码
    Ring_VirtualMachine* rvm = get_global_rvm();
    rvm->current_coroutine = g;
    
    // 保存机器状态，以便在协程切换时恢复
    // ...
    
    // 执行协程直到yield或完成
    // ...
    
    // 协程执行完成或被挂起
    if (g->status == G_STATUS_DEAD) {
        // 协程完成，清理资源
        finish_g(m, g);
    } else if (g->status == G_STATUS_SUSPENDED) {
        // 协程被挂起，加入到对应的等待队列
        // ...
    }
    
    // 重置机器状态
    m->running_g = nullptr;
}
```

### 协程切换机制

#### 1. yield 实现
```cpp
int yield_g(Ring_VirtualMachine* rvm) {
    RingCoroutine* g = rvm->current_coroutine;
    Machine* m = get_machine_by_id(g->m_id);
    
    // 记录当前协程的状态
    g->status = G_STATUS_RUNNABLE;
    g->call_info->coroutine_resume_pc = g->call_info->pc + 1;
    
    // 将协程加入到本地队列
    std::lock_guard<std::mutex> lock(m->local_queue_mutex);
    m->local_runnable_gs.push_back(g);
    
    // 让出CPU，让机器调度其他协程
    m->running_g = nullptr;
    
    return 0;
}
```

#### 2. resume 实现
```cpp
int resume_g(Ring_VirtualMachine* rvm, CO_ID target_g_id) {
    // 在新的设计中，resume主要是将协程标记为可运行状态
    RingCoroutine* target_g = coroutine_map[target_g_id];
    if (target_g == nullptr || target_g->status == G_STATUS_DEAD) {
        return -1;
    }
    
    // 标记协程为可运行状态
    target_g->status = G_STATUS_RUNNABLE;
    
    // 将协程加入到全局队列
    Scheduler* sched = get_global_scheduler();
    std::lock_guard<std::mutex> lock(sched->global_queue_mutex);
    sched->global_runnable_gs.push_back(target_g);
    sched->global_runnable_count++;
    
    // 唤醒挂起的机器
    wakeup_parked_machines(sched);
    
    return 0;
}
```

### 虚拟机与协程的集成

1. 修改 `ring_virtualmachine_create` 函数，初始化调度器和主机器：
```cpp
Ring_VirtualMachine* ring_virtualmachine_create() {
    // 现有代码...
    
    // 初始化调度器
    Scheduler* sched = init_scheduler();
    set_global_scheduler(sched);
    
    // 创建主机器
    Machine* main_m = create_machine(sched, true);
    set_global_main_machine(main_m);
    
    return rvm;
}
```

2. 修改 `ring_virtualmachine_init` 函数，创建并启动根协程：
```cpp
void ring_virtualmachine_init(Ring_VirtualMachine* rvm) {
    // 现有代码...
    
    // 创建并启动根协程
    RingCoroutine* root_g = launch_root_coroutine(rvm);
    
    // 将根协程添加到主机器的本地队列
    Machine* main_m = get_global_main_machine();
    std::lock_guard<std::mutex> lock(main_m->local_queue_mutex);
    main_m->local_runnable_gs.push_back(root_g);
    
    // 执行根协程
    execute_g(main_m, root_g);
}
```

### 实现步骤

1. 首先，更新 `ring.hpp` 中的数据结构定义，添加 Machine 和 Scheduler 结构体
2. 实现调度器的基本功能，包括创建机器、管理全局和本地队列
3. 修改现有的协程创建和切换函数，使其与新的调度器兼容
4. 实现工作窃取和机器挂起/唤醒机制
5. 更新虚拟机初始化和执行逻辑，集成新的协程系统
6. 添加必要的锁和同步机制，确保线程安全

### 测试与验证

1. 编写测试用例，验证新的协程系统的基本功能
2. 测试多协程并发执行的性能和正确性
3. 验证工作窃取机制是否正常工作
4. 测试在高负载情况下系统的稳定性

### 注意事项

1. 确保线程安全，特别是在访问共享数据结构时
2. 避免死锁和竞争条件
3. 注意内存管理，及时清理不再使用的资源
4. 确保协程切换的性能，避免不必要的开销
5. 保持与现有代码的兼容性