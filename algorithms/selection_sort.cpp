#include <vector>
#include <test_sort.h>

namespace
{
	void selection_sort(std::vector<int> *const v)
	{
		std::vector<int> &u = *v;
		size_t p = u.size();
		while (1 < p--)
		{
			int mx = p;
			for (size_t i = 0; p != i; ++i)
			{
				if (u[mx] < u[i])
				{
					mx = i;
				}
			}
			std::swap(u[mx], u[p]);
		}
	}
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, {{selection_sort, "selection_sort"}});
}

