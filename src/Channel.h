#ifndef MINI_RPG_CHANNEL_H
#define MINI_RPG_CHANNEL_H

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

// A safe queue for communicating events between the threads
// Inspired by https://st.xorian.net/blog/2012/08/go-style-channel-in-c/
template <class type>
class Channel {
    std::list<type> queue_;
    std::mutex qLock_;
    std::condition_variable qCV_;

public:
    Channel() = default;

    void send(const type &t) {
        std::unique_lock<std::mutex> lock(qLock_);  // RAII-style mutex
        queue_.push_back(t);
        qCV_.notify_one();
    }

    bool receive(type &target, bool block = false) {
        std::unique_lock<std::mutex> lock(qLock_);

        if (block) {
            // block the thread until there is something to receive
            qCV_.wait(lock, [&](){ return !queue_.empty(); });
        } else if (queue_.empty()) {
            return false;
        }

        target = queue_.front();
        queue_.pop_front();
        return true;
    }
};

#endif //MINI_RPG_CHANNEL_H
