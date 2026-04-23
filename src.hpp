#pragma once
#include <new>
#include <utility>

namespace sjtu {

template <typename _Tp>
class unique_ptr {
  private:
    _Tp *ptr;

  public:
    unique_ptr() : ptr(nullptr) {}

    unique_ptr(const unique_ptr &) = delete;

    unique_ptr(unique_ptr &&other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    ~unique_ptr() {
        delete ptr;
    }

    unique_ptr &operator=(const unique_ptr &) = delete;

    unique_ptr &operator=(unique_ptr &&other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    explicit unique_ptr(_Tp *p) : ptr(p) {}

    void reset() {
        delete ptr;
        ptr = nullptr;
    }

    _Tp *release() {
        _Tp *temp = ptr;
        ptr = nullptr;
        return temp;
    }

    _Tp *get() const {
        return ptr;
    }

    _Tp &operator*() {
        return *ptr;
    }

    _Tp *operator->() {
        return ptr;
    }
};

static_assert(sizeof(unique_ptr<int>) <= sizeof(void *));

template <typename _Tp, typename... Args>
unique_ptr<_Tp> make_unique(Args &&... args) {
    return unique_ptr<_Tp>(new _Tp(std::forward<Args>(args)...));
}

} // namespace sjtu
