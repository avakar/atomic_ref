#ifndef AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X64_h
#define AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X64_h

#include "atomic_ref.msvc.x86_x64.h"

#include <atomic>
#include <type_traits>
#include <intrin.h>

namespace _avakar {
namespace atomic_ref {

template <typename T>
struct is_always_lock_free
	: std::integral_constant<bool, sizeof(T) <= 8>
{
};

template <typename T>
struct is_always_wait_free
	: std::integral_constant<bool, sizeof(T) <= 8>
{
};

template <typename T>
auto load(T const & obj, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 8, T>
{
	T val = obj;
	if (order != std::memory_order_relaxed)
		_ReadWriteBarrier();
	return val;
}

template <typename T>
auto store(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 8>
{
	if (order != std::memory_order_relaxed)
		_ReadWriteBarrier();
	obj = desired;
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedExchange64((long long *)&obj, (long long &)desired);
	return (T &)r;
}

template <typename T>
auto fetch_add(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedExchangeAdd64((long long *)&obj, (long long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedExchangeAdd64((long long *)&obj, -(long long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_add(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	(void)order;
	return (T *)_InterlockedExchangeAdd64((long long *)&obj, arg * sizeof(T));
}

template <typename T>
auto fetch_sub(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	(void)order;
	return (T *)_InterlockedExchangeAdd64((long long *)&obj, -arg * sizeof(T));
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedAnd64((long long *)&obj, (long long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedOr64((long long *)&obj, (long long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	(void)order;
	long long r = _InterlockedXor64((long long *)&obj, (long long &)arg);
	return (T &)r;
}

}
}

#endif // _h
