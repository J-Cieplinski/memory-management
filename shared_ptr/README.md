<!-- .slide: data-background="#111111" -->

# Homework

___

## Pre-work

* Read the [presentation about move semantics and std::forward](https://github.com/coders-school/modern-cpp/raw/master/module3/move_semantics.pdf)
* Read the [variadic template part of this presentation](https://github.com/coders-school/kurs_cpp_lato_2019/raw/master/L07-modern-cpp/modern_cpp.pdf)

It will be useful for a post-work ;)

___

## Post-work

* `shared_ptr` (50 XP)
* `weak_ptr` (25 XP) for volunteers
* `make_shared` (40 XP) for volunteers

### Bonuses

* +3 XP for delivery of each task before 11.10.2020 23:59
* +3 XP for cooperation for each person in the group

___
<!-- .slide: style="font-size: 0.78em" -->

### `shared_ptr`

(50 XP) Implement your own `shared_ptr` (simplified).

`shared_ptr` is a RAII class:

* Holds a pointer to managed object (template class)
* Holds a pointer to shared control block with 2 counters and a deleter:
  * shared_refs count (as `std::atomic<size_t>`)
  * weak_refs count (as `std::atomic<size_t>`)
  * deleter (function pointer)
* Constructor copies a pointer and allocate the control block
* Destructor decrease shared_refs and:
  * if shared_refs == 0 -> release the managed object
  * if shared_refs == 0 and weak_refs == 0 -> release the control block
* Copying is allowed - it increments shared_refs
* Moving is allowed and it means:
  * Copying original pointers to a new object
  * Setting source pointer to nullptr
* Member functions: `operator*()`, `operator->()`, `get()`, `reset()`, `use_count()`, `operator bool()`

Do not forget about CI - UT + Valgrind / ASAN. Work in pairs.

[Read the notes on cppreference.com](https://en.cppreference.com/w/cpp/memory/shared_ptr)

___

### `weak_ptr` - for volunteers

(25 XP) Implement your own `weak_ptr`

* Proper cooperation with above `shared_ptr`
* Member functions: `use_count()`, `expired()`, `lock()`, `reset()`

Think about proper constructors / assignment operators or visit cppreference.com :)

___

### `make_shared` - for volunteers (level hard)

(40 XP + achievement) Implement `make_shared` function.

It should:

* create a `shared_ptr` of a given type
* use variadic templates to take any number of parameters
* first template parameter type is the underlying object type
* other template parameters are passed via perfect forwarding to the constructor of a given type

Think about optimized memory layout of `shared_ptr` created by `make_shared`. How to achieve it?
