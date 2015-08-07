#pragma once

#include <vector>
#include <initializer_list>

struct test_sort_item
{
	void (*const f)(std::vector<int> *v);
	const char *const name;
};

bool test_sort(const std::initializer_list<test_sort_item> items);
int test_sort_main(int argc, char *argv[],
				   const std::initializer_list<test_sort_item> items);
