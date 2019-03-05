# atomic_ref

Header-only implementation of [std::atomic_ref][1] for C++14.
Currently supports Microsoft Visual Studio on x86 and amd64,
and GCC/clang with support for __atomic builtins.

  [1]: https://en.cppreference.com/w/cpp/atomic/atomic_ref

## Getting started

Place the library anywhere and add the `include` directory to your
project's include paths.

Bring `avakar::atomic_ref` to your namespace and enjoy.

    #include <avakar/atomic_ref.h>
    using avakar::atomic_ref;

## License

The software is distributed under the terms of the MIT license.

## Lock- and wait-freeness

The standard mandates a constexpr bool member called `is_always_lock_free`.
This implementation also defines a bool constexpr member `is_always_wait_free`.
Note that in particular, operations on `uint64_t` in Intel's 32-bit platforms
are not wait free.

## Safe variant

The library also defines the class `avakar::safe_atomic_ref`,
which forces you to use the more verbose function calls insted of operators.
In particual, `safe_atomic_ref` doesn't provide

* implicit conversion operator (`operator T`),
* pre- and post-increment,
* pre- and post-decrement,
* the assignment operator, or
* any of the compound assignment operators.
