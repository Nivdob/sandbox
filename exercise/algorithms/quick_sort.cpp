#include <vector>
#include <test_sort.h>

namespace
{
	template <typename I>
	I pivot(const I b, const I e)
	{
		return b + (e - b) / 2;
	}

	template <typename I>
	void quick_sort_i(const I b, const I e)
	{
		const size_t d = e - b;
		if (1 >= d)
		{
			return;
		}
		const I p = pivot(b, e);
		I pp = e - 1;
		std::swap(*p, *pp);
		I k = b;
		for (I i = b; pp != i; ++i)
		{
			if (*i < *pp)
			{
				std::swap(*k, *i);
				++k;
			}
		}
		std::swap(*pp, *k);
		quick_sort_i(b, k);
		quick_sort_i(k + 1, e);
	}

	void quick_sort(std::vector<int> *const v)
	{
		quick_sort_i(v->begin(), v->end());
	}
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, {{quick_sort, "quick_sort"}});
}

