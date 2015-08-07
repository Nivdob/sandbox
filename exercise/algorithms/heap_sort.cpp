#include <vector>
#include <test_sort.h>

namespace
{
	void heapify(std::vector<int> *const v)
	{
		std::vector<int> &u = *v;
		if (1 >= u.size())
		{
			return;
		}
		for (size_t k = (u.size() - 2) / 2 + 1; 0 < k--;)
		{
			size_t k1 = 2*k + 1;
			size_t k2 = 2*k + 2;
			if (u.size() > k1 && u[k] > u[k1])
			{
				std::swap(u[k], u[k1]);
			}
			if (u.size() > k2 && u[k] > u[k2])
			{
				std::swap(u[k], u[k2]);
			}
		}
	}

	void heap_sort(std::vector<int> *const v)
	{
		heapify(v);
	}
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, {{heap_sort, "heap_sort"}});
}

