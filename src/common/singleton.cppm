
export module singleton;

namespace hybridsearch {

export template <typename T>
class Singleton {
public:
    inline static T &instance() {
        static T instance;
        return instance;
    }

    virtual ~Singleton() = default;
};

} // namespace hybridsearch
