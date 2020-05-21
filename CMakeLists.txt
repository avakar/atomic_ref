cmake_minimum_required(VERSION 3.12)
project(avakar_atomic_ref)
include(FetchContent)

add_library(avakar_atomic_ref INTERFACE)
target_include_directories(avakar_atomic_ref INTERFACE include)
target_compile_features(avakar_atomic_ref INTERFACE cxx_std_14)

add_library(avakar::atomic_ref ALIAS avakar_atomic_ref)

include(CTest)
if (BUILD_TESTING)
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG d10b9bd02e098476670f5eb0527d2c7281476e8a
		GIT_SHALLOW YES
		)

	FetchContent_GetProperties(Catch2)
	if (NOT catch2_POPULATED)
		FetchContent_Populate(Catch2)
		add_subdirectory("${catch2_SOURCE_DIR}" "${catch2_BINARY_DIR}")
	endif()

	add_executable(avakar_atomic_ref_test
		test/main.cpp
		test/test.cpp
		)
	target_link_libraries(avakar_atomic_ref_test avakar::atomic_ref Catch2::Catch2)

	add_test(NAME avakar::atomic_ref COMMAND avakar_atomic_ref_test)
endif()
