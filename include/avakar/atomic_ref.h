#ifndef AVAKAR_ATOMIC_REF_h
#define AVAKAR_ATOMIC_REF_h

#include <atomic>
#include <cstddef>

#if defined(_MSC_VER) && defined(_M_IX86)
#include "../../src/atomic_ref.msvc.x86.h"
#elif defined(_MSC_VER) && defined(_M_AMD64)
#include "../../src/atomic_ref.msvc.x64.h"
#else
#error Unsupported platform
#endif

namespace avakar {

template <typename T, typename = void>
struct _atomic_ref;

template <typename T>
using safe_atomic_ref = _atomic_ref<T>;

template <typename T>
struct atomic_ref
	: _atomic_ref<T>
{
	using difference_type = typename _atomic_ref<T>::difference_type;

	explicit atomic_ref(T & obj)
		: _atomic_ref<T>(obj)
	{
	}

	operator T() const noexcept
	{
		return this->load();
	}

	T operator=(T desired) const noexcept
	{
		this->store(desired);
		return desired;
	}

	T operator++() const noexcept
	{
		return this->fetch_add(1) + T(1);
	}

	T operator++(int) const noexcept
	{
		return this->fetch_add(1);
	}

	T operator--() const noexcept
	{
		return this->fetch_sub(1) - T(1);
	}

	T operator--(int) const noexcept
	{
		return this->fetch_sub(1);
	}

	T operator+=(difference_type arg) const noexcept
	{
		return this->fetch_add(arg) + arg;
	}

	T operator-=(difference_type arg) const noexcept
	{
		return this->fetch_sub(arg) - arg;
	}

	T operator&=(T arg) const noexcept
	{
		return this->fetch_and(arg) & arg;
	}

	T operator|=(T arg) const noexcept
	{
		return this->fetch_or(arg) | arg;
	}

	T operator^=(T arg) const noexcept
	{
		return this->fetch_xor(arg) ^ arg;
	}
};


template <typename T, typename>
struct _atomic_ref
{
	static_assert(std::is_trivially_copyable_v<T>);

	static constexpr bool is_always_lock_free = _avakar::atomic_ref::is_always_lock_free_v<T>;
	static constexpr bool is_always_wait_free = _avakar::atomic_ref::is_always_wait_free_v<T>;
	static constexpr std::size_t required_alignment = alignof(T);

	using value_type = T;

	explicit _atomic_ref(value_type & obj)
		: _obj(obj)
	{
	}

	value_type load(std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::load(_obj, order);
	}

	void store(value_type desired, std::memory_order order = std::memory_order_seq_cst) noexcept
	{
		_avakar::atomic_ref::store(_obj, desired, order);
	}

	value_type exchange(value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::exchange(_obj, desired, order);
	}

	bool compare_exchange_weak(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_weak(expected, desired, order, order);
	}

	bool compare_exchange_weak(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_weak(_obj, expected, desired, success, failure);
	}

	bool compare_exchange_strong(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_strong(expected, desired, order, order);
	}

	bool compare_exchange_strong(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_strong(_obj, expected, desired, success, failure);
	}

	_atomic_ref & operator=(_atomic_ref const &) = delete;

private:
	value_type & _obj;
};

template <typename T>
struct _atomic_ref<T *>
{
	static_assert(std::is_trivially_copyable_v<T>);

	static constexpr bool is_always_lock_free = _avakar::atomic_ref::is_always_lock_free_v<T>;
	static constexpr bool is_always_wait_free = _avakar::atomic_ref::is_always_wait_free_v<T>;
	static constexpr std::size_t required_alignment = alignof(T);

	using value_type = T *;
	using difference_type = std::ptrdiff_t;

	explicit _atomic_ref(value_type & obj)
		: _obj(obj)
	{
	}

	value_type load(std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::load(_obj, order);
	}

	void store(value_type desired, std::memory_order order = std::memory_order_seq_cst) noexcept
	{
		_avakar::atomic_ref::store(_obj, desired, order);
	}

	value_type exchange(value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::exchange(_obj, desired, order);
	}

	bool compare_exchange_weak(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_weak(expected, desired, order, order);
	}

	bool compare_exchange_weak(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_weak(_obj, expected, desired, success, failure);
	}

	bool compare_exchange_strong(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_strong(expected, desired, order, order);
	}

	bool compare_exchange_strong(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_strong(_obj, expected, desired, success, failure);
	}

	value_type fetch_add(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_add(_obj, arg, order);
	}

	value_type fetch_sub(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_sub(_obj, arg, order);
	}

	_atomic_ref & operator=(_atomic_ref const &) = delete;

private:
	value_type & _obj;
};

template <typename T>
struct _atomic_ref<T, std::enable_if_t<std::is_integral_v<T>>>
{
	static_assert(std::is_trivially_copyable_v<T>);

	static constexpr bool is_always_lock_free = _avakar::atomic_ref::is_always_lock_free_v<T>;
	static constexpr bool is_always_wait_free = _avakar::atomic_ref::is_always_wait_free_v<T>;
	static constexpr std::size_t required_alignment = alignof(T);

	using value_type = T;
	using difference_type = T;

	explicit _atomic_ref(value_type & obj)
		: _obj(obj)
	{
	}

	value_type load(std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::load(_obj, order);
	}

	void store(value_type desired, std::memory_order order = std::memory_order_seq_cst) noexcept
	{
		_avakar::atomic_ref::store(_obj, desired, order);
	}

	value_type exchange(value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::exchange(_obj, desired, order);
	}

	bool compare_exchange_weak(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_weak(expected, desired, order, order);
	}

	bool compare_exchange_weak(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_weak(_obj, expected, desired, success, failure);
	}

	bool compare_exchange_strong(value_type & expected, value_type desired, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return this->compare_exchange_strong(expected, desired, order, order);
	}

	bool compare_exchange_strong(
		value_type & expected, value_type desired,
		std::memory_order success,
		std::memory_order failure) const noexcept
	{
		return _avakar::atomic_ref::compare_exchange_strong(_obj, expected, desired, success, failure);
	}

	value_type fetch_add(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_add(_obj, arg, order);
	}

	value_type fetch_sub(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_sub(_obj, arg, order);
	}

	value_type fetch_and(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_and(_obj, arg, order);
	}

	value_type fetch_or(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_or(_obj, arg, order);
	}

	value_type fetch_xor(difference_type arg, std::memory_order order = std::memory_order_seq_cst) const noexcept
	{
		return _avakar::atomic_ref::fetch_xor(_obj, arg, order);
	}

	_atomic_ref & operator=(_atomic_ref const &) = delete;

private:
	value_type & _obj;
};

}

#endif // _h
