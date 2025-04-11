

module;

export module defer_op;

import stl;

namespace hybridsearch {

export template <typename FN>
class DeferFn {
public:
    explicit DeferFn(FN func) : func_(std::move(func)) {}

    DeferFn(const DeferFn &) = delete;
    DeferFn(DeferFn &&) : func_(std::exchange(func_, None)) {}

    ~DeferFn() noexcept {
        if (func_) {
            (*func_)();
        }
    }

private:
    Optional<FN> func_;
};

} // namespace hybridsearch