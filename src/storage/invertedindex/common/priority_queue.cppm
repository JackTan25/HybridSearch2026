

module;

export module priority_queue;

import stl;

namespace hybridsearch {

export template <class Type>
class PriorityQueue {

private:
    Type *heap_{nullptr};
    u32 size_{0};
    u32 max_size_{0};

    void UpHeap() {
        u32 i = size_;
        Type node = heap_[i];
        u32 j = i >> 1;
        while (j > 0 && LessThan(node, heap_[j])) {
            heap_[i] = heap_[j];
            i = j;
            j = j >> 1;
        }
        heap_[i] = node;
    }
    void DownHeap() {
        u32 i = 1;
        Type node = heap_[i];
        u32 j = i << 1;
        u32 k = j + 1;
        if (k <= size_ && LessThan(heap_[k], heap_[j])) {
            j = k;
        }
        while (j <= size_ && LessThan(heap_[j], node)) {
            heap_[i] = heap_[j];
            i = j;
            j = i << 1;
            k = j + 1;
            if (k <= size_ && LessThan(heap_[k], heap_[j])) {
                j = k;
            }
        }
        heap_[i] = node;
    }

protected:
    PriorityQueue() {}

    virtual bool LessThan(Type a, Type b) = 0;

    void Initialize(const u32 max_size) {
        size_ = 0;
        max_size_ = max_size;
        u32 heap_size = max_size_ + 1;
        heap_ = new Type[heap_size];
    }

public:
    virtual ~PriorityQueue() { delete[] heap_; }

    void Put(Type element) {
        size_++;
        heap_[size_] = element;
        UpHeap();
    }

    bool Insert(Type element) {
        if (size_ < max_size_) {
            Put(element);
            return true;
        } else if (size_ > 0 && !LessThan(element, Top())) {
            heap_[1] = element;
            AdjustTop();
            return true;
        } else
            return false;
    }

    Type Top() {
        if (size_ > 0)
            return heap_[1];
        else
            return Type();
    }

    Type Pop() {
        if (size_ > 0) {
            Type result = heap_[1];
            heap_[1] = heap_[size_];

            heap_[size_] = (Type)0;
            size_--;
            DownHeap();
            return result;
        } else
            return Type();
    }

    void AdjustTop() { DownHeap(); }

    u32 Size() { return size_; }

    Type operator[](u32 pos) { return heap_[pos + 1]; }

    Type At(u32 pos) { return heap_[pos + 1]; }
};

} // namespace hybridsearch
