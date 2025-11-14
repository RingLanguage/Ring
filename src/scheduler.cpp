#include "ring.hpp"
#include <cassert>
#include <ctime>
#include <iostream>
#include <unistd.h> // for usleep

// 全局变量
Scheduler*  global_scheduler    = nullptr;
Machine*    global_main_machine = nullptr;
static M_ID global_machine_id   = 0;

// 获取下一个机器ID
static M_ID get_next_machine_id() {
    return global_machine_id++;
}

// 函数前向声明
void* machine_run(void* arg);
void  execute_g(Machine* m, RingCoroutine* g);
void  add_g_to_local_queue(Machine* m, RingCoroutine* g);
void  wakeup_parked_machines(Scheduler* sched);

// 获取全局调度器
Scheduler* get_global_scheduler() {
    return global_scheduler;
}

// 设置全局调度器
void set_global_scheduler(Scheduler* sched) {
    global_scheduler = sched;
}

// 获取全局主机器
Machine* get_global_main_machine() {
    return global_main_machine;
}

// 设置全局主机器
void set_global_main_machine(Machine* m) {
    global_main_machine = m;
}

// 根据ID获取机器
Machine* get_machine_by_id(M_ID m_id) {
    if (global_scheduler == nullptr)
        return nullptr;

    for (auto m : global_scheduler->machines) {
        if (m->m_id == m_id) {
            return m;
        }
    }

    return nullptr;
}

/*
 * 初始化调度器
 */
Scheduler* init_scheduler() {
    Scheduler* sched             = (Scheduler*)mem_alloc(NULL_MEM_POOL, sizeof(Scheduler));
    sched->global_runnable_count = 0;
    return sched;
}

/*
 * 创建机器（物理线程）
 */
Machine* create_machine(Scheduler* sched, bool is_main) {
    Machine* m    = (Machine*)mem_alloc(NULL_MEM_POOL, sizeof(Machine));
    m->m_id       = get_next_machine_id();
    m->running_g  = nullptr;
    m->is_main    = is_main;
    m->park_count = 0;

    // 将机器添加到调度器
    sched->machines.push_back(m);

    if (!is_main) {
        // 创建并启动物理线程
        pthread_create(&m->thread_id, NULL, machine_run, m);
    }

    return m;
}

/*
 * 窃取工作机制
 */
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
        if (other_m == m)
            continue;

        std::lock_guard<std::mutex> other_lock(other_m->local_queue_mutex);
        if (other_m->local_runnable_gs.size() > 1) {
            // 窃取一半的协程
            size_t steal_count = other_m->local_runnable_gs.size() / 2;
            for (size_t i = 0; i < steal_count; i++) {
                m->local_runnable_gs.push_back(other_m->local_runnable_gs.front());
                other_m->local_runnable_gs.erase(other_m->local_runnable_gs.begin());
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

/*
 * 机器运行函数
 */
void* machine_run(void* arg) {
    Machine*             m     = (Machine*)arg;
    Scheduler*           sched = get_global_scheduler();
    Ring_VirtualMachine* rvm   = nullptr; // 这里需要获取全局虚拟机

    while (true) {
        // 从本地队列获取可运行的协程
        RingCoroutine* g = nullptr;

        // 尝试从本地队列获取
        {
            std::lock_guard<std::mutex> lock(m->local_queue_mutex);
            if (!m->local_runnable_gs.empty()) {
                g = m->local_runnable_gs.back();
                m->local_runnable_gs.pop_back();
            }
        }

        if (g == nullptr) {
            // 本地队列为空，尝试从全局队列获取
            g = steal_work(sched, m);
            if (g == nullptr) {
                // 没有可运行的协程，挂起
                // park_machine(m);
                // 简单的实现，暂时使用sleep
                usleep(1000); // 1ms
                continue;
            }
        }

        // 执行协程
        execute_g(m, g);
    }

    return nullptr;
}

/*
 * 协程执行函数
 */
void execute_g(Machine* m, RingCoroutine* g) {
    // 获取全局虚拟机
    Ring_VirtualMachine* rvm = nullptr; // 全局虚拟机，实际应从上下文获取

    // 设置运行状态
    g->status    = CO_STAT_RUNNING;
    g->g_status  = G_STATUS_RUNNING;
    m->running_g = g;
    g->m_id      = m->m_id;

    // 记录开始运行时间
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    g->last_run_time = (long long)(ts.tv_sec) * 1000000000 + ts.tv_nsec;

    // 执行协程代码
    if (rvm != nullptr) {
        rvm->current_coroutine = g;
        // 注意：实际的协程执行逻辑应该在虚拟机中实现
        // 这里只是设置当前协程，具体的执行由虚拟机完成
    }

    // 重置机器状态
    m->running_g = nullptr;
}

/*
 * 让出协程的执行权
 */
void yield_g(RingCoroutine* g) {
    // 获取当前机器
    Machine* m = get_machine_by_id(g->m_id);
    if (m == nullptr)
        return;

    // 设置协程状态为可运行
    g->status   = CO_STAT_SUSPENDED;
    g->g_status = G_STATUS_RUNNABLE;

    // 将协程添加到本地队列末尾
    add_g_to_local_queue(m, g);

    // 重置机器的运行协程
    m->running_g = nullptr;
}

/*
 * 恢复协程执行
 */
void resume_g(RingCoroutine* g) {
    // 获取当前机器
    Machine* m = get_global_main_machine();
    if (m == nullptr) {
        m = get_machine_by_id(g->m_id);
    }

    if (m == nullptr)
        return;

    // 设置协程状态为可运行
    g->status   = CO_STAT_SUSPENDED;
    g->g_status = G_STATUS_RUNNABLE;

    // 将协程添加到本地队列末尾
    add_g_to_local_queue(m, g);

    // 唤醒机器执行
    wakeup_parked_machines(global_scheduler);
}

/*
 * 唤醒挂起的机器
 */
void wakeup_parked_machines(Scheduler* sched) {
    // 简单实现，实际应该实现条件变量来唤醒挂起的机器
}

/*
 * 将协程添加到全局队列
 */
void add_g_to_global_queue(RingCoroutine* g) {
    if (global_scheduler == nullptr)
        return;

    std::lock_guard<std::mutex> lock(global_scheduler->global_queue_mutex);
    global_scheduler->global_runnable_gs.push_back(g);
    global_scheduler->global_runnable_count++;

    // 唤醒挂起的机器
    wakeup_parked_machines(global_scheduler);
}

/*
 * 将协程添加到本地队列
 */
void add_g_to_local_queue(Machine* m, RingCoroutine* g) {
    std::lock_guard<std::mutex> lock(m->local_queue_mutex);
    m->local_runnable_gs.push_back(g);
}