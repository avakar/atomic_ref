#include <atomic>
#include <type_traits>

namespace _avakar {
namespace atomic_ref {

template <typename T>
struct is_always_lock_free
	: std::integral_constant<bool, __atomic_always_lock_free(sizeof(T), 0)>
{
};

template <typename T>
struct is_always_wait_free
	: std::false_type
{
};

template <typename T>
T load(T const & obj, std::memory_order order) noexcept
{
	return __atomic_load_n(&obj, order);
}

template <typename T>
void store(T & obj, T desired, std::memory_order order) noexcept
{
	return __atomic_store(&obj, &order);
}

template <typename T>
T exchange(T & obj, T desired, std::memory_order order) noexcept
{
	return __atomic_exchange_n(&obj, desired, order);
}

template <typename T>
bool compare_exchange_weak(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
{
	return __atomic_compare_exchange(&obj, &expected, &desired, true, success, failure);
}

template <typename T>
bool compare_exchange_strong(T & obj, T & expected, T desired, std::memory_order success, std::memory_order failure) noexcept
{
	return __atomic_compare_exchange(&obj, &expected, &desired, false, success, failure);
}

template <typename T>
T fetch_add(T & obj, T arg, std::memory_order order) noexcept
{
	return __atomic_fetch_add(&obj, arg, order);
}

template <typename T>
T fetch_sub(T & obj, T arg, std::memory_order order) noexcept
{
	return __atomic_fetch_sub(&obj, arg, order);
}

template <typename T>
T fetch_and(T & obj, T arg, std::memory_order order) noexcept
{
	return __atomic_fetch_and(&obj, arg, order);
}

template <typename T>
T fetch_or(T & obj, T arg, std::memory_order order) noexcept
{
	return __atomic_fetch_or(&obj, arg, order);
}

template <typename T>
T fetch_xor(T & obj, T arg, std::memory_order order) noexcept
{
	return __atomic_fetch_xor(&obj, arg, order);
}

}
}
