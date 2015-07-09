#include <vector>
#include <test_sort.h>

namespace
{
	void recombine(std::vector<int> &vi, std::vector<int> &vo,
				   const size_t b, const size_t e)
	{
		const size_t n = e - b;
		if (1 >= n)
		{
			return;
		}
		const size_t m = b + n / 2;
		recombine(vo, vi, b, m);
		recombine(vo, vi, m, e);
		for (size_t i = b, j = m, k = b; e > k; ++k)
		{
			if (j == e || (i != m && vi[i] < vi[j]))
			{
				vo[k] = vi[i++];
			}
			else
			{
				vo[k] = vi[j++];
			}
		}

	}

	void merge_sort(std::vector<int> *const v)
	{
		std::vector<int> u = *v;
		recombine(*v, u, 0, v->size());
		*v = u;
	}

	void merge_sort_nr(std::vector<int> *const v)
	{
		std::vector<int> &vi = *v;
		std::vector<int> vo = vi;
		const size_t n = v->size();
		for (size_t i = 1, d = 2; n > i; d = 2 * (i = d))
		{
			for (size_t j = 0; n > j; j += d)
			{
				const size_t b = j;
				const size_t m = std::min(n, j + i);
				const size_t e = std::min(n, j + d);
				for (size_t k = b, ki = b, kj = m; e > k; ++k)
				{
					if (ki == m || (kj != e && vi[ki] > vi[kj]))
					{
						vo[k] = vi[kj++];
					}
					else
					{
						vo[k] = vi[ki++];
					}
				}
			}
			std::swap(vi, vo);
		}
	}
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, {
			{merge_sort, "merge_sort"},
			{merge_sort_nr, "merge_sort_nr"}
	});
}

