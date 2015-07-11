#include <cstdio>
#include "countof.h"
#include "array_file.h"
#include "test_sort.h"

namespace
{
	struct test_case
	{
		const std::vector<int> input;
		const std::vector<int> output;
	};

	const test_case c_test_cases[] =
	{
		{{}, {}},
		{{42}, {42}},
		{{1, 2}, {1, 2}},
		{{2, 1}, {1, 2}},
		{{1, 3, 2}, {1, 2, 3}},
		{{2, 3, 1, 4}, {1, 2, 3, 4}},
		{{0, 3, 9, 8, 5}, {0, 3, 5, 8, 9}},
		{{0, 3, 9, 8, 6, 5}, {0, 3, 5, 6, 8, 9}},
		{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
		{{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
		{{1, 1, 9, 9, 0, 2, 3, 4, 3, 2}, {0, 1, 1, 2, 2, 3, 3, 4, 9, 9}},
		{{1, 7, 2, 9, 4, 8, 5, 6, 3, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	};
}

bool test_sort(const std::initializer_list<test_sort_item> items)
{
	for(auto i = items.begin(), e = items.end(); e != i; ++i)
	{
		for (size_t k = 0; _countof(c_test_cases) > k; ++k)
		{
			const test_case &tc = c_test_cases[k];
			std::vector<int> v = tc.input;
			i->f(&v);
			if (tc.output != v)
			{
				std::fprintf(stderr, "%s: test case #%zu failed\n",
							 0 != i->name? i->name: "sort", k);
				std::fprintf(stderr, "\tInput:\n\t");
				array_write(stderr, &tc.input);
				std::fprintf(stderr, "\tExpected output:\n\t");
				array_write(stderr, &tc.output);
				std::fprintf(stderr, "\tActual output:\n\t");
				array_write(stderr, &v);
				return false;
			}
		}
	}
	return true;
}

int test_sort_main(int argc, char *argv[],
				   const std::initializer_list<test_sort_item> items)
{
	if (2 == argc)
	{
		if (0 == strcmp("test", argv[1]))
		{
			return test_sort(items)? 0: -1;
		}
		std::vector<int> v;
		array_read(argv[1], &v);
		for(auto i = items.begin(), e = items.end(); e != i; ++i)
		{
			std::vector<int> u = v;
			i->f(&v);
			array_write(stdout, &u);
		}
		return 0;
	}
	return -1;
}
