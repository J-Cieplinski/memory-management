#pragma once

#include <algorithm>
#include "control_block.hpp"
#include "shared_ptr.hpp"

namespace cs {
template <typename T>
class weak_ptr {
public:
    constexpr weak_ptr() noexcept = default;
    weak_ptr(const weak_ptr& ptr) noexcept;
    weak_ptr(const shared_ptr<T>& ptr) noexcept;
    weak_ptr(weak_ptr&& previousOwner) noexcept;
    ~weak_ptr();

    weak_ptr& operator=(const weak_ptr& ptr) noexcept;
    weak_ptr& operator=(const shared_ptr<T>& ptr) noexcept;
    weak_ptr& operator=(weak_ptr&& previousOwner) noexcept;

    void reset() noexcept;
    bool expired() const noexcept;
    shared_ptr<T> lock() const noexcept;
    size_t use_count() { return counter_->getRefs(); } 
    void swap(weak_ptr& ptr) noexcept;

private:
    control_block<T>* counter_{nullptr};
    T* ptr_{nullptr};

    void checkAndDeletePointers();
};


template <typename T>
void weak_ptr<T>::checkAndDeletePointers() {
    if (!counter_->getWeakRefs() && !counter_->getRefs()) {
            delete counter_;
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& ptr) noexcept
    : ptr_{ptr.ptr_}, counter_{ptr.counter_} {
    if (counter_) {
        counter_->increaseWeakRefs();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& sPtr) noexcept
    : ptr_{sPtr.ptr_}, counter_{sPtr.counter_} {
    if (counter_) {
        counter_->increaseWeakRefs();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr&& previousOwner) noexcept
    : ptr_(previousOwner.ptr_), counter_(previousOwner.counter_) {
    previousOwner.ptr_ = nullptr;
    previousOwner.counter_ = nullptr;
}

template <typename T>
weak_ptr<T>::~weak_ptr() {
    if (counter_ != nullptr) {
        counter_->decreaseWeakRefs();
        checkAndDeletePointers();
    }
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& ptr) noexcept {
    if (this != &ptr) {
        if(counter_) {
            counter_->decreaseWeakRefs();
            checkAndDeletePointers();
        }
    ptr_ = ptr.ptr_;
    counter_ = ptr.counter_;
        counter_->increaseWeakRefs();
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<T>& ptr) noexcept {
    if(counter_) {
        counter_->decreaseWeakRefs();
        checkAndDeletePointers();
    }
    ptr_ = ptr.ptr_;
    counter_ = ptr.counter_;
    if (counter_) {
        counter_->increaseWeakRefs();
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& previousOwner) noexcept {
    if (this != &previousOwner) {
        weak_ptr(previousOwner).swap(*this);
        previousOwner.ptr_ = nullptr;
        previousOwner.counter_ = nullptr;
    }
    return *this;
}

template <typename T>
void weak_ptr<T>::reset() noexcept {
    weak_ptr{}.swap(*this);
}

template <typename T>
bool weak_ptr<T>::expired() const noexcept {
    return (counter_ == nullptr || counter_->getRefs() == 0);
}

template <typename T>
shared_ptr<T> weak_ptr<T>::lock() const noexcept {
    return expired() ? cs::shared_ptr<T>() : cs::shared_ptr<T>(ptr_, counter_);
}

template <typename T>
void weak_ptr<T>::swap(weak_ptr& ptr) noexcept {
    std::swap(ptr_, ptr.ptr_);
    std::swap(counter_, ptr.counter_);
}

} //namespace cs
