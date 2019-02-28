#include <avakar/atomic_ref.h>
#include <stdint.h>

int main()
{
	using avakar::atomic_ref;

	uint64_t k = 0;
	atomic_ref<uint64_t> ak(k);

	uint64_t * ptr = &k;
	atomic_ref<uint64_t *> aptr(ptr);

	auto ppp = aptr.fetch_add(1);

	return (int)ak.exchange(1);
}
