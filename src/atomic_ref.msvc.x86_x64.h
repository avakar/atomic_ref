#include <atomic>
#include <type_traits>
#include <intrin.h>

namespace _avakar::atomic_ref {

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	char r = _InterlockedExchange8((char *)&obj, (char &)desired);
	return (T &)r;
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	short r = _InterlockedExchange16((short *)&obj, (short &)desired);
	return (T &)r;
}

template <typename T>
auto exchange(T & obj, T desired, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	long r = _InterlockedExchange((long *)&obj, (long &)desired);
	return (T &)r;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 1, bool>
{
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
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	if constexpr (sizeof(T) == 1)
	{
		char r = _InterlockedExchangeAdd8((char *)&obj, (char &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 2)
	{
		short r = _InterlockedExchangeAdd16((short *)&obj, (short &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 4)
	{
		long r = _InterlockedExchangeAdd((long *)&obj, (long &)arg);
		return (T &)r;
	}
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	if constexpr (sizeof(T) == 1)
	{
		char r = _InterlockedExchangeAdd8((char *)&obj, -(char &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 2)
	{
		short r = _InterlockedExchangeAdd16((short *)&obj, -(short &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 4)
	{
		long r = _InterlockedExchangeAdd((long *)&obj, -(long &)arg);
		return (T &)r;
	}
}

template <typename T>
auto fetch_and(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	if constexpr (sizeof(T) == 1)
	{
		char r = _InterlockedAnd8((char *)&obj, -(char &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 2)
	{
		short r = _InterlockedAnd16((short *)&obj, -(short &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 4)
	{
		long r = _InterlockedAnd((long *)&obj, -(long &)arg);
		return (T &)r;
	}
}

template <typename T>
auto fetch_or(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	if constexpr (sizeof(T) == 1)
	{
		char r = _InterlockedOr8((char *)&obj, -(char &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 2)
	{
		short r = _InterlockedOr16((short *)&obj, -(short &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 4)
	{
		long r = _InterlockedOr((long *)&obj, -(long &)arg);
		return (T &)r;
	}
}

template <typename T>
auto fetch_xor(T & obj, T arg, std::memory_order order) noexcept
	-> std::enable_if_t<sizeof(T) <= 4, T>
{
	if constexpr (sizeof(T) == 1)
	{
		char r = _InterlockedXor8((char *)&obj, -(char &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 2)
	{
		short r = _InterlockedXor16((short *)&obj, -(short &)arg);
		return (T &)r;
	}
	else if constexpr (sizeof(T) == 4)
	{
		long r = _InterlockedXor((long *)&obj, -(long &)arg);
		return (T &)r;
	}
}

}
