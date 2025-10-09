#include "ring.hpp"
#include <cassert>
#include <iostream>

// 创建一个新的通道
Channel* create_channel(size_t buffer_size) {
    Channel* ch     = (Channel*)mem_alloc(NULL_MEM_POOL, sizeof(Channel));
    ch->buffer_size = buffer_size;
    ch->closed      = false;
    return ch;
}

// 关闭一个通道
void close_channel(Channel* ch) {
    if (ch == nullptr)
        return;

    std::lock_guard<std::mutex> lock(ch->mutex);
    if (ch->closed)
        return; // 通道已经关闭

    ch->closed = true;

    // 唤醒所有等待的协程
    ch->send_cond.notify_all();
    ch->recv_cond.notify_all();

    // 处理所有等待的协程，将它们重新加入到调度队列
    Machine* main_m = get_global_main_machine();

    while (!ch->send_wait_gs.empty()) {
        RingCoroutine* g = ch->send_wait_gs.front();
        ch->send_wait_gs.pop_front();
        add_g_to_global_queue(g);
    }

    while (!ch->recv_wait_gs.empty()) {
        RingCoroutine* g = ch->recv_wait_gs.front();
        ch->recv_wait_gs.pop_front();
        add_g_to_global_queue(g);
    }
}

// 向通道发送数据
bool send_to_channel(Channel* ch, const RVM_Value& value) {
    if (ch == nullptr)
        return false;

    std::unique_lock<std::mutex> lock(ch->mutex);

    // 检查通道是否已关闭
    if (ch->closed)
        return false;

    // 获取当前协程
    RingCoroutine*       current_g = nullptr;
    Ring_VirtualMachine* rvm       = nullptr; // 实际应该从全局获取虚拟机
    if (rvm != nullptr) {
        current_g = rvm->current_coroutine;
    }

    // 如果是无缓冲通道或者缓冲区已满，等待接收方
    while (ch->buffer.size() >= ch->buffer_size) {
        // 如果没有接收方在等待且通道未关闭，将当前协程加入等待队列并让出执行权
        if (!ch->recv_wait_gs.empty()) {
            // 有接收方在等待，直接传递数据
            RingCoroutine* recv_g = ch->recv_wait_gs.front();
            ch->recv_wait_gs.pop_front();

            // 这里应该将数据传递给接收方，但由于我们使用的是简单的实现，
            // 先放入缓冲区，让接收方自己获取
            ch->buffer.push_back(value);

            // 唤醒接收方
            add_g_to_global_queue(recv_g);

            return true;
        } else if (current_g == nullptr) {
            // 不是在协程中调用，无法等待
            return false;
        } else {
            // 将当前协程加入发送等待队列
            ch->send_wait_gs.push_back(current_g);

            // 让出协程执行权
            lock.unlock();
            yield_g(current_g);

            // 重新获取锁
            lock.lock();

            // 检查通道是否已关闭
            if (ch->closed)
                return false;
        }
    }

    // 将数据放入缓冲区
    ch->buffer.push_back(value);

    // 通知等待的接收方
    ch->recv_cond.notify_one();

    return true;
}

// 从通道接收数据
bool recv_from_channel(Channel* ch, RVM_Value* value) {
    if (ch == nullptr || value == nullptr)
        return false;

    std::unique_lock<std::mutex> lock(ch->mutex);

    // 获取当前协程
    RingCoroutine*       current_g = nullptr;
    Ring_VirtualMachine* rvm       = nullptr; // 实际应该从全局获取虚拟机
    if (rvm != nullptr) {
        current_g = rvm->current_coroutine;
    }

    // 如果缓冲区为空，等待发送方
    while (ch->buffer.empty()) {
        // 如果通道已关闭，返回false
        if (ch->closed)
            return false;

        if (!ch->send_wait_gs.empty()) {
            // 有发送方在等待，直接接收数据
            RingCoroutine* send_g = ch->send_wait_gs.front();
            ch->send_wait_gs.pop_front();

            // 这里应该直接从发送方获取数据，但由于我们使用的是简单的实现，
            // 让发送方先放入缓冲区，然后我们再获取

            // 唤醒发送方
            add_g_to_global_queue(send_g);

            // 重新检查缓冲区
            if (ch->buffer.empty()) {
                if (current_g == nullptr)
                    return false;

                // 将当前协程加入接收等待队列
                ch->recv_wait_gs.push_back(current_g);

                // 让出协程执行权
                lock.unlock();
                yield_g(current_g);

                // 重新获取锁
                lock.lock();

                // 检查通道是否已关闭
                if (ch->closed)
                    return false;
            }
        } else if (current_g == nullptr) {
            // 不是在协程中调用，无法等待
            return false;
        } else {
            // 将当前协程加入接收等待队列
            ch->recv_wait_gs.push_back(current_g);

            // 让出协程执行权
            lock.unlock();
            yield_g(current_g);

            // 重新获取锁
            lock.lock();

            // 检查通道是否已关闭
            if (ch->closed)
                return false;
        }
    }

    // 从缓冲区获取数据
    *value = ch->buffer.front();
    ch->buffer.pop_front();

    // 通知等待的发送方
    ch->send_cond.notify_one();

    return true;
}