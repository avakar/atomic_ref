#ifndef AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X64_h
#define AVAKAR_ATOMIC_REF_ATOMIC_REF_MSVC_X64_h

#include <type_traits>
#include <intrin.h>
#include <arm64intr.h>

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
auto _load_relaxed(T const & obj) noexcept
	-> std::enable_if_t<sizeof(T) == 1, T>
{
	auto r = __iso_volatile_load8(reinterpret_cast<char const volatile *>(&obj));
	return reinterpret_cast<T &>(r);
}

template <typename T>
auto _load_relaxed(T const & obj) noexcept
	-> std::enable_if_t<sizeof(T) == 2, T>
{
	auto r = __iso_volatile_load16(reinterpret_cast<short const volatile *>(&obj));
	return reinterpret_cast<T &>(r);
}

template <typename T>
auto _load_relaxed(T const & obj) noexcept
	-> std::enable_if_t<sizeof(T) == 4, T>
{
	auto r = __iso_volatile_load32(reinterpret_cast<int const volatile *>(&obj));
	return reinterpret_cast<T &>(r);
}

template <typename T>
auto _load_relaxed(T const & obj) noexcept
	-> std::enable_if_t<sizeof(T) == 8, T>
{
	auto r = __iso_volatile_load64(reinterpret_cast<long long const volatile *>(&obj));
	return reinterpret_cast<T &>(r);
}

template <typename T>
auto load(T const & obj, std::memory_order order) noexcept
	-> decltype(_load_relaxed(obj))
{
	T val = _load_relaxed(obj);
	if (order != std::memory_order_relaxed)
		__dmb(_ARM64_BARRIER_ISH);
	return val;
}

template <typename T>
auto _store_relaxed(T & obj, T desired) noexcept
	-> std::enable_if_t<sizeof(T) == 1>
{
	__iso_volatile_store8(
		reinterpret_cast<char volatile *>(&obj),
		reinterpret_cast<char const &>(desired));
}

template <typename T>
auto _store_relaxed(T & obj, T desired) noexcept
	-> std::enable_if_t<sizeof(T) == 2>
{
	__iso_volatile_store16(
		reinterpret_cast<short volatile *>(&obj),
		reinterpret_cast<short const &>(desired));
}

template <typename T>
auto _store_relaxed(T & obj, T desired) noexcept
	-> std::enable_if_t<sizeof(T) == 4>
{
	__iso_volatile_store32(
		reinterpret_cast<int volatile *>(&obj),
		reinterpret_cast<int const &>(desired));
}

template <typename T>
auto _store_relaxed(T & obj, T desired) noexcept
	-> std::enable_if_t<sizeof(T) == 8>
{
	__iso_volatile_store64(
		reinterpret_cast<long long volatile *>(&obj),
		reinterpret_cast<long long const &>(desired));
}

template <typename T>
auto store(T & obj, T desired, std::memory_order order) noexcept
	-> decltype(_store_relaxed(obj, desired))
{
	switch (order)
	{
	case std::memory_order_relaxed:
		_store_relaxed(obj, desired);
		break;

	case std::memory_order_release:
		__dmb(_ARM64_BARRIER_ISH);
		_store_relaxed(obj, desired);
		break;

	case std::memory_order_seq_cst:
		__dmb(_ARM64_BARRIER_ISH);
		_store_relaxed(obj, desired);
		__dmb(_ARM64_BARRIER_ISH);
		break;
	}
}

#define X(fnname, intr, width, type) \
	template <typename T> \
	auto fnname(T & obj, T arg, std::memory_order order) noexcept \
		-> std::enable_if_t<sizeof(T) == width, T> \
	{ \
		type r; \
		switch (order) \
		{ \
		case std::memory_order_relaxed: \
			r = intr ## _nf( \
				reinterpret_cast<type volatile *>(&obj), \
				reinterpret_cast<type const &>(arg)); \
			break; \
		case std::memory_order_consume: \
		case std::memory_order_acquire: \
			r = intr ## _acq( \
				reinterpret_cast<type volatile *>(&obj), \
				reinterpret_cast<type const &>(arg)); \
			break; \
		case std::memory_order_release: \
			r = intr ## _rel( \
				reinterpret_cast<type volatile *>(&obj), \
				reinterpret_cast<type const &>(arg)); \
			break; \
		default: \
			r = intr( \
				reinterpret_cast<type volatile *>(&obj), \
				reinterpret_cast<type const &>(arg)); \
		} \
		return reinterpret_cast<T &>(r); \
	}

X(exchange, _InterlockedExchange8, 1, char)
X(exchange, _InterlockedExchange16, 2, short)
X(exchange, _InterlockedExchange, 4, long)
X(exchange, _InterlockedExchange64, 8, long long)

X(fetch_add, _InterlockedExchangeAdd8, 1, char)
X(fetch_add, _InterlockedExchangeAdd16, 2, short)
X(fetch_add, _InterlockedExchangeAdd, 4, long)
X(fetch_add, _InterlockedExchangeAdd64, 8, long long)

X(fetch_and, _InterlockedAnd8, 1, char)
X(fetch_and, _InterlockedAnd16, 2, short)
X(fetch_and, _InterlockedAnd, 4, long)
X(fetch_and, _InterlockedAnd64, 8, long long)

X(fetch_or, _InterlockedOr8, 1, char)
X(fetch_or, _InterlockedOr16, 2, short)
X(fetch_or, _InterlockedOr, 4, long)
X(fetch_or, _InterlockedOr64, 8, long long)

X(fetch_xor, _InterlockedXor8, 1, char)
X(fetch_xor, _InterlockedXor16, 2, short)
X(fetch_xor, _InterlockedXor, 4, long)
X(fetch_xor, _InterlockedXor64, 8, long long)

#undef X

inline constexpr std::memory_order _combine_memory_order(std::memory_order a, std::memory_order b)
{
	static_assert(std::memory_order_release > std::memory_order_acquire, "Unexpected memory_order ordering");

	if (a < b)
		std::swap(a, b);

	if (a == std::memory_order_release
		&& (b == std::memory_order_acquire || b == std::memory_order_consume))
	{
		return std::memory_order_acq_rel;
	}

	return a;
}

template <typename T>
auto compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 1, bool>
{
	char exp = reinterpret_cast<char &>(expected);
	char prev;

	switch (_combine_memory_order(success, failure))
	{
	case std::memory_order_relaxed:
		prev = _InterlockedCompareExchange8_nf(
			reinterpret_cast<char volatile *>(&obj),
			reinterpret_cast<char &>(desired),
			exp);
		break;
	case std::memory_order_consume:
	case std::memory_order_acquire:
		prev = _InterlockedCompareExchange8_acq(
			reinterpret_cast<char volatile *>(&obj),
			reinterpret_cast<char &>(desired),
			exp);
		break;
	case std::memory_order_release:
		prev = _InterlockedCompareExchange8_rel(
			reinterpret_cast<char volatile *>(&obj),
			reinterpret_cast<char &>(desired),
			exp);
		break;
	default:
		prev = _InterlockedCompareExchange8(
			reinterpret_cast<char volatile *>(&obj),
			reinterpret_cast<char &>(desired),
			exp);
	}

	if (exp == prev)
		return true;

	expected = reinterpret_cast<T &>(prev);
	return false;
}

template <typename T>
auto compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 2, bool>
{
	short exp = reinterpret_cast<short &>(expected);
	short prev;

	switch (_combine_memory_order(success, failure))
	{
	case std::memory_order_relaxed:
		prev = _InterlockedCompareExchange16_nf(
			reinterpret_cast<short volatile *>(&obj),
			reinterpret_cast<short &>(desired),
			exp);
		break;
	case std::memory_order_consume:
	case std::memory_order_acquire:
		prev = _InterlockedCompareExchange16_acq(
			reinterpret_cast<short volatile *>(&obj),
			reinterpret_cast<short &>(desired),
			exp);
		break;
	case std::memory_order_release:
		prev = _InterlockedCompareExchange16_rel(
			reinterpret_cast<short volatile *>(&obj),
			reinterpret_cast<short &>(desired),
			exp);
		break;
	default:
		prev = _InterlockedCompareExchange16(
			reinterpret_cast<short volatile *>(&obj),
			reinterpret_cast<short &>(desired),
			exp);
	}

	if (exp == prev)
		return true;

	expected = reinterpret_cast<T &>(prev);
	return false;
}

template <typename T>
auto compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 4, bool>
{
	long exp = reinterpret_cast<long &>(expected);
	long prev;

	switch (_combine_memory_order(success, failure))
	{
	case std::memory_order_relaxed:
		prev = _InterlockedCompareExchange_nf(
			reinterpret_cast<long volatile *>(&obj),
			reinterpret_cast<long &>(desired),
			exp);
		break;
	case std::memory_order_consume:
	case std::memory_order_acquire:
		prev = _InterlockedCompareExchange_acq(
			reinterpret_cast<long volatile *>(&obj),
			reinterpret_cast<long &>(desired),
			exp);
		break;
	case std::memory_order_release:
		prev = _InterlockedCompareExchange_rel(
			reinterpret_cast<long volatile *>(&obj),
			reinterpret_cast<long &>(desired),
			exp);
		break;
	default:
		prev = _InterlockedCompareExchange(
			reinterpret_cast<long volatile *>(&obj),
			reinterpret_cast<long &>(desired),
			exp);
	}

	if (exp == prev)
		return true;

	expected = reinterpret_cast<T &>(prev);
	return false;
}

template <typename T>
auto compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> std::enable_if_t<sizeof(T) == 8, bool>
{
	long long exp = reinterpret_cast<long long &>(expected);
	long long prev;

	switch (_combine_memory_order(success, failure))
	{
	case std::memory_order_relaxed:
		prev = _InterlockedCompareExchange64_nf(
			reinterpret_cast<long long volatile *>(&obj),
			reinterpret_cast<long long &>(desired),
			exp);
		break;
	case std::memory_order_consume:
	case std::memory_order_acquire:
		prev = _InterlockedCompareExchange64_acq(
			reinterpret_cast<long long volatile *>(&obj),
			reinterpret_cast<long long &>(desired),
			exp);
		break;
	case std::memory_order_release:
		prev = _InterlockedCompareExchange64_rel(
			reinterpret_cast<long long volatile *>(&obj),
			reinterpret_cast<long long &>(desired),
			exp);
		break;
	default:
		prev = _InterlockedCompareExchange64(
			reinterpret_cast<long long volatile *>(&obj),
			reinterpret_cast<long long &>(desired),
			exp);
	}

	if (exp == prev)
		return true;

	expected = reinterpret_cast<T &>(prev);
	return false;
}

template <typename T>
auto compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
	-> decltype(compare_exchange_strong(obj, expected, desired, success, failure))
{
	return compare_exchange_strong(obj, expected, desired, success, failure);
}

template <typename T>
auto fetch_sub(T & obj, T arg, std::memory_order order) noexcept
	-> decltype(fetch_add<T>(obj, -arg, order))
{
	return fetch_add<T>(obj, ~arg + 1, order);
}

template <typename T>
auto fetch_add(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	long long r = fetch_add(
		reinterpret_cast<long long &>(obj),
		static_cast<long long>(arg * sizeof(T)),
		order);
	return reinterpret_cast<T *>(r);
}

template <typename T>
auto fetch_sub(T * & obj, std::ptrdiff_t arg, std::memory_order order) noexcept
{
	long long r = fetch_sub(
		reinterpret_cast<long long &>(obj),
		static_cast<long long>(arg * sizeof(T)),
		order);
	return reinterpret_cast<T *>(r);
}

}
}

#endif // _h
