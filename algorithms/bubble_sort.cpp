#include <cstdlib>
#include <algorithm>
#include <vector>
#include <test_sort.h>

namespace
{
	void bubble_sort(std::vector<int> *const v)
	{
		const auto b = v->begin();
		auto e = v->end();
		for(bool sorted = b == e; !sorted;)
		{
			sorted = true;
			auto i = b, j = b + 1;
			for (; e != j; i = j++)
			{
				if (*i > *j)
				{
					std::swap(*i, *j);
					sorted = false;
				}
			}
			e = i;
		}
	}

	void insertion_sort(std::vector<int> *const v)
	{
		std::vector<int> &u = *v;
		for (size_t i = 1; v->size() > i; ++i)
		{
			int x = u[i];
			size_t j;
			for (j = i; 0 < j-- && u[j] > x;)
			{
				u[j + 1] = u[j];
				u[j] = x;
			}
		}
	}

	bool test_sorts()
	{
		if (!test_sort(bubble_sort, "bubble_sort")) return false;
		if (!test_sort(insertion_sort, "insertion_sort")) return false;
		return true;
	}
}

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;
	if (!test_sorts())
	{
		return -1;
	}
	return 0;
}
