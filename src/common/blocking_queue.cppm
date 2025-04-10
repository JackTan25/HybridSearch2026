

module;

export module blocking_queue;

import stl;
import default_values;
import global_resource_usage;
import third_party;

namespace hybridsearch {

export template <typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(const String &name, SizeT capacity = DEFAULT_BLOCKING_QUEUE_SIZE) : capacity_(capacity) {
        name_ = name;
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount(fmt::format("BlockingQueue: {}", name_));
#endif
    }
    ~BlockingQueue() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount(fmt::format("BlockingQueue: {}", name_));
#endif
    }

    void NotAllowEnqueue() { allow_enqueue_ = false; }

    bool Enqueue(T &task) {
        {
            if (!allow_enqueue_) {
                return false;
            }

            std::unique_lock<std::mutex> lock(queue_mutex_);
            full_cv_.wait(lock, [this] { return queue_.size() < capacity_; });
            queue_.push_back(task);
        }
        empty_cv_.notify_one();
        return true;
    }

    bool Enqueue(T &&task) {
        {
            if (!allow_enqueue_) {
                return false;
            }

            std::unique_lock<std::mutex> lock(queue_mutex_);
            full_cv_.wait(lock, [this] { return queue_.size() < capacity_; });
            queue_.push_back(std::forward<T>(task));
        }
        empty_cv_.notify_one();
        return true;
    }

    void EnqueueBulk(Vector<T> &input_array) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            full_cv_.wait(lock, [&] { return queue_.size() + input_array.size() < capacity_; });
            queue_.insert(queue_.end(), input_array.begin(), input_array.end());
        }
        empty_cv_.notify_one();
    }

    void EnqueueBulk(List<T> &input_queue) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            full_cv_.wait(lock, [&] { return queue_.size() + input_queue.size() < capacity_; });
            queue_.splice(queue_.end(), input_queue);
        }
        empty_cv_.notify_one();
    }

    void Dequeue(T &task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            empty_cv_.wait(lock, [this] { return !queue_.empty(); });
            task = queue_.front();
            queue_.pop_front();
        }
        full_cv_.notify_one();
    }

    T DequeueReturn() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        empty_cv_.wait(lock, [this] { return !queue_.empty(); });
        T res = std::move(queue_.front());
        queue_.pop_front();
        full_cv_.notify_one();
        return res;
    }

    void DequeueBulk(List<T> &output_queue) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            empty_cv_.wait(lock, [this] { return !queue_.empty(); });
            output_queue.splice(output_queue.end(), queue_);
        }
        full_cv_.notify_one();
    }

    void DequeueBulk(Vector<T> &output_array) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            empty_cv_.wait(lock, [this] { return !queue_.empty(); });
            output_array.insert(output_array.end(), queue_.begin(), queue_.end());
            queue_.clear();
        }
        full_cv_.notify_one();
    }

    void DequeueBulk(Deque<T> &output_array) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            empty_cv_.wait(lock, [this] { return !queue_.empty(); });
            output_array.swap(queue_);
            //            output_array.insert(output_array.end(), queue_.begin(), queue_.end());
            queue_.clear();
        }
        full_cv_.notify_one();
    }

    bool TryDequeue(T &task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (queue_.empty()) {
                return false;
            }
            task = queue_.front();
            queue_.pop_front();
        }
        full_cv_.notify_one();
        return true;
    }

    bool TryDequeueBulk(Vector<T> &output_array) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (queue_.empty()) {
                return false;
            }
            output_array.insert(output_array.end(), queue_.begin(), queue_.end());
            queue_.clear();
        }
        full_cv_.notify_one();
        return true;
    }

    [[nodiscard]] SizeT Size() const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return queue_.size();
    }

    [[nodiscard]] bool Empty() const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return queue_.empty();
    }

protected:
    atomic_bool allow_enqueue_{true};
    mutable std::mutex queue_mutex_{};
    std::condition_variable full_cv_{};
    std::condition_variable empty_cv_{};
    Deque<T> queue_{};
    SizeT capacity_{DEFAULT_BLOCKING_QUEUE_SIZE};
    String name_{};
};

} // namespace hybridsearch
