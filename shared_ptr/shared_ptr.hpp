#pragma once

#include "control_block.hpp"

#include <stdexcept>
#include <functional>
namespace cs {
    
template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
public:
    template <typename B> friend class weak_ptr;
    template<typename D, typename... Args> friend shared_ptr<D> make_shared(Args&& ... args);
    explicit shared_ptr() = default;
    explicit shared_ptr(T* ptr);
    shared_ptr(T* ptr, std::function<void(T*)> deleter);
    shared_ptr(std::nullptr_t, std::function<void(T*)> deleter);
    shared_ptr(const shared_ptr& ptr) noexcept;  //copy c-tor
    shared_ptr(shared_ptr&& previousOwner) noexcept;      //move c-tor
    ~shared_ptr();

    void swap(shared_ptr<T>& secondPointer) noexcept;
    const T* get() const;
    void reset(T* newPtr = nullptr, std::function<void(T*)> deleter = [](T* ptr) { delete ptr;});

    const T* operator->();
    T& operator*();
    explicit operator bool() const noexcept;
    shared_ptr<T>& operator=(const shared_ptr<T>& ptr) noexcept;             //copy assignment
    shared_ptr<T>& operator=(shared_ptr<T>&& previousOwner) noexcept;  //move assignment

    size_t use_count() { return counter_->getRefs(); }

private:
    shared_ptr(T* ptr, control_block<T>* counter);
    T* ptr_{nullptr};
    control_block<T>* counter_{nullptr};

    void checkAndDeletePointers();
};

template <typename T>
void shared_ptr<T>::checkAndDeletePointers() {
    if (!counter_->getRefs()) {
            counter_->deleter_(ptr_);
        if (!counter_->getWeakRefs()) {
            delete counter_;
        }
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr, control_block<T>* counter) : ptr_(ptr), counter_(counter) {
    ++*counter_;
}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr)
    : ptr_(ptr) {
    if (ptr_) {
        counter_ = new control_block<T>();
        ++(*counter_);
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr, std::function<void(T*)> deleter)
    : ptr_(ptr) {
    if (ptr_) {
        counter_ = new control_block<T>(deleter);
        ++(*counter_);
    }
}
template <typename T>
shared_ptr<T>::shared_ptr(std::nullptr_t, std::function<void(T*)> deleter) {
    counter_ = new control_block<T>(deleter);
    ++(*counter_);
}


template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& ptr) noexcept
    : counter_(ptr.counter_), ptr_(ptr.ptr_) {
    ++(*counter_);
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& previousOwner) noexcept
    :   ptr_(previousOwner.ptr_), counter_(previousOwner.counter_) {
    previousOwner.ptr_ = nullptr;
    previousOwner.counter_ = nullptr;
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    if (counter_ != nullptr) {
        --*counter_;
        checkAndDeletePointers();
    }
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr<T>& secondPointer) noexcept {
    auto ptrTmp = secondPointer.ptr_;
    auto counterTmp = secondPointer.counter_;
    secondPointer.ptr_ = ptr_;
    secondPointer.counter_ = counter_;
    ptr_ = ptrTmp;
    counter_ = counterTmp;
}

template <typename T>
const T* shared_ptr<T>::get() const {
    return ptr_;
}

template <typename T>
void shared_ptr<T>::reset(T* newPtr, std::function<void(T*)> deleter) {
    shared_ptr{newPtr, deleter}.swap(*this);
}

template <typename T>
const T* shared_ptr<T>::operator->() {
    return ptr_;
}

template <typename T>
T& shared_ptr<T>::operator*() {
    if(!ptr_) {
        throw std::runtime_error("Dereferencing a nullptr");
    }
    return *ptr_;
}

template <typename T>
shared_ptr<T>::operator bool() const noexcept {
    return ptr_;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& previousOwner) noexcept{
    if (this != &previousOwner) {
        if(counter_) {
            --*counter_;
            checkAndDeletePointers();
        }

        ptr_ = previousOwner.ptr_;
        counter_ = previousOwner.counter_;
        previousOwner.ptr_ = nullptr;
        previousOwner.counter_ = nullptr;
    }

    return *this;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& ptr) noexcept {
    if(counter_){
        --*counter_;
        checkAndDeletePointers();
    }

    counter_ = ptr.counter_;
    ptr_ = ptr.ptr_;
    ++(*counter_);

    return *this;
}

template<typename D, typename... Args>
shared_ptr<D> make_shared(Args&&... args) {
    auto tempCounter = new continuous_block<D>(std::forward<Args>(args)...);
    return shared_ptr<D>(tempCounter->getObjectPointer(), tempCounter);
}

}  // namespace cs
