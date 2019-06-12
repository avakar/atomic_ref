#ifndef AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X86_X64_h
#define AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X86_X64_h

#include <atomic>
#include <type_traits>
#include <intrin.h>

namespace _avakar {
namespace atomic_ref {

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedExchange8((char *)&obj, (char &)desired);
	return (T &)r;
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedExchange16((short *)&obj, (short &)desired);
	return (T &)r;
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedExchange((long *)&obj, (long &)desired);
	return (T &)r;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 1, bool>
{
	(void)success;
	(void)failure;

	char exp = (char &)expected;
	char prev = _InterlockedCompareExchange8((char *)&obj, (char &)desired, exp);
	if (exp == prev)
		return true;

	expected = (T &)prev;
	return false;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 2, bool>
{
	(void)success;
	(void)failure;

	short exp = (short &)expected;
	short prev = _InterlockedCompareExchange16((short *)&obj, (short &)desired, exp);
	if (exp == prev)
		return true;

	expected = (T &)prev;
	return false;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 4, bool>
{
	(void)success;
	(void)failure;

	long exp = (long &)expected;
	long prev = _InterlockedCompareExchange((long *)&obj, (long &)desired, exp);
	if (exp == prev)
		return true;

	expected = (T &)prev;
	return false;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 8, bool>
{
	(void)success;
	(void)failure;

	long long exp = (long long &)expected;
	long long prev = _InterlockedCompareExchange64((long long *)&obj, (long long &)desired, exp);
	if (exp == prev)
		return true;

	expected = (T &)prev;
	return false;
}

template <typename T>
bool compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
{
	return compare_exchange_weak(obj, expected, desired, success, failure);
}

template <typename T>
auto fetch_add(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedExchangeAdd8((char *)&obj, (char &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_add(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedExchangeAdd16((short *)&obj, (short &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_add(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedExchangeAdd((long *)&obj, (long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedExchangeAdd8((char *)&obj, -(char &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedExchangeAdd16((short *)&obj, -(short &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedExchangeAdd((long *)&obj, -(long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedAnd8((char *)&obj, (char &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedAnd16((short *)&obj, (short &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedAnd((long *)&obj, (long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedOr8((char *)&obj, (char &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedOr16((short *)&obj, (short &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedOr((long *)&obj, (long &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	(void)order;
	char r = _InterlockedXor8((char *)&obj, (char &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	(void)order;
	short r = _InterlockedXor16((short *)&obj, (short &)arg);
	return (T &)r;
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	(void)order;
	long r = _InterlockedXor((long *)&obj, (long &)arg);
	return (T &)r;
}

}
}

#endif // _h
