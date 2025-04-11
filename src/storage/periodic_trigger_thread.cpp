

module;

#include <thread>
#include <vector>

module periodic_trigger_thread;

import stl;
import logger;

namespace hybridsearch {

PeriodicTriggerThread::~PeriodicTriggerThread() {
    if (running_) {
        Stop();
    }
}

void PeriodicTriggerThread::Start() {
    LOG_INFO("Periodic trigger start ...");
    running_ = true;
    thread_ = Thread([this] { Run(); });
}

void PeriodicTriggerThread::Stop() {
    {
        std::lock_guard lock(mtx_);
        running_ = false;
        cv_.notify_all();
    }
    thread_.join();
    LOG_INFO("Periodic trigger stop ...");
}

void PeriodicTriggerThread::Run() {
    auto sleep_time = std::chrono::milliseconds(1000);
    while (true) {
        std::unique_lock lock(mtx_);
        cv_.wait_for(lock, sleep_time, [this] { return !running_; });
        if (!running_) {
            break;
        }
        if (cleanup_trigger_ != nullptr && cleanup_trigger_->Check()) {
            cleanup_trigger_->Trigger();
        }

        if (full_checkpoint_trigger_ != nullptr && full_checkpoint_trigger_->Check()) {
            full_checkpoint_trigger_->Trigger();
        }

        if (delta_checkpoint_trigger_ != nullptr && delta_checkpoint_trigger_->Check()) {
            delta_checkpoint_trigger_->Trigger();
        }

        if (compact_segment_trigger_ != nullptr && compact_segment_trigger_->Check()) {
            compact_segment_trigger_->Trigger();
        }

        if (optimize_index_trigger_ != nullptr && optimize_index_trigger_->Check()) {
            optimize_index_trigger_->Trigger();
        }
    }
}

} // namespace hybridsearch
