#include <vector>
#include <test_sort.h>

namespace
{
	void insertion_sort(std::vector<int> *const v)
	{
		std::vector<int> &u = *v;
		for (size_t i = 1, n = v->size(); n > i; ++i)
		{
			int x = u[i];
			size_t j;
			for (j = i; 0 < j && u[j - 1] > x; --j)
			{
				u[j] = u[j - 1];
			}
			u[j] = x;
		}
	}
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, {{insertion_sort, "insertion_sort"}});
}

