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
		{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
		{{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	};
}
bool test_sort(void (*f)(std::vector<int> *v), const char *const name)
{
	for (size_t i = 0; _countof(c_test_cases) > i; ++i)
	{
		const test_case &tc = c_test_cases[i];
		std::vector<int> v = tc.input;
		f(&v);
		if (tc.output != v)
		{
			std::fprintf(stderr, "%s: test case #%zu failed\n",
						 0 != name? name: "sort", i);
			std::fprintf(stderr, "\tInput:\n\t");
			array_write(stderr, &tc.input);
			std::fprintf(stderr, "\tExpected output:\n\t");
			array_write(stderr, &tc.output);
			std::fprintf(stderr, "\tActual output:\n\t");
			array_write(stderr, &v);
			return false;
		}
	}
	return true;
}
