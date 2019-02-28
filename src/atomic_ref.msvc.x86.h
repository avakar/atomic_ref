#include "atomic_ref.msvc.x86_x64.h"

#include <atomic>
#include <type_traits>
#include <intrin.h>

namespace _avakar::atomic_ref {

template <typename T>
constexpr bool is_always_lock_free_v = sizeof(T) <= 8;

template <typename T>
constexpr bool is_always_wait_free_v = sizeof(T) <= 4;

template <typename T>
auto load(T const & obj, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	T val = obj;
	if (order != std::memory_order_relaxed)
		_ReadWriteBarrier();
	return val;
}

template <typename T>
auto store(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4>
{
	if (order != std::memory_order_relaxed)
		_ReadWriteBarrier();
	obj = desired;
}

template <typename T>
auto load(T const & obj, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	T exp = {};
	compare_exchange_weak(obj, exp, exp, order, order);
	return exp;
}

template <typename T>
auto store(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8>
{
	exchange(obj, desired, order);
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	long long exp = (long long &)obj;
	for (;;)
	{
		long long prev = _InterlockedCompareExchange64((long long *)&obj, (long long &)desired, exp);
		if (prev == exp)
			return prev;
		exp = prev;
	}
}

template <typename T>
auto fetch_add(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	_ReadWriteBarrier();
	T exp = obj;
	while (!compare_exchange_weak(obj, exp, exp + arg, order, order))
	{
	}
	_ReadWriteBarrier();
	return exp;
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	_ReadWriteBarrier();
	T exp = obj;
	while (!compare_exchange_weak(obj, exp, exp - arg, order, order))
	{
	}
	_ReadWriteBarrier();
	return exp;
}

template <typename T>
auto fetch_add(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	return (T *)_InterlockedExchangeAdd((long *)&obj, arg * sizeof(T));
}

template <typename T>
auto fetch_sub(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	return (T *)_InterlockedExchangeAdd((long *)&obj, -arg * sizeof(T));
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	_ReadWriteBarrier();
	T exp = obj;
	while (!compare_exchange_weak(obj, exp, exp & arg, order, order))
	{
	}
	_ReadWriteBarrier();
	return exp;
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	_ReadWriteBarrier();
	T exp = obj;
	while (!compare_exchange_weak(obj, exp, exp | arg, order, order))
	{
	}
	_ReadWriteBarrier();
	return exp;
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	_ReadWriteBarrier();
	T exp = obj;
	while (!compare_exchange_weak(obj, exp, exp ^ arg, order, order))
	{
	}
	_ReadWriteBarrier();
	return exp;
}

}
