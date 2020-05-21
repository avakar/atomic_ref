#include <avakar/atomic_ref.h>
#include <catch2/catch.hpp>
using avakar::atomic_ref;

TEST_CASE("Pointer add/sub is correct")
{
	int v = 0;
	atomic_ref<int> a(v);

	REQUIRE(a.fetch_add(1) == 0);
	REQUIRE(a.fetch_add(1) == 1);
	REQUIRE(v == 2);
	REQUIRE(a.fetch_sub(1) == 2);
	REQUIRE(a.fetch_sub(1) == 1);
	REQUIRE(v == 0);

	REQUIRE(a.fetch_add(2) == 0);
	REQUIRE(v == 2);
	REQUIRE(a.fetch_sub(2) == 2);
	REQUIRE(v == 0);
}

TEST_CASE("Pointer arithmetic is correct")
{
	int arr[] = { 1, 2, 3 };
	int * p = arr;
	atomic_ref<int *> a(p);

	REQUIRE(a.fetch_add(1) == &arr[0]);
	REQUIRE(a.fetch_add(1) == &arr[1]);
	REQUIRE(p == &arr[2]);
	REQUIRE(a.fetch_sub(1) == &arr[2]);
	REQUIRE(a.fetch_sub(1) == &arr[1]);
	REQUIRE(p == &arr[0]);

	REQUIRE(a.fetch_add(2) == &arr[0]);
	REQUIRE(p == &arr[2]);
	REQUIRE(a.fetch_sub(2) == &arr[2]);
	REQUIRE(p == &arr[0]);
}
