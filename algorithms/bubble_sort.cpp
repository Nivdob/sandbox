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
}

int main(int argc, char *argv[])
{
	return test_sort_main(argc, argv, bubble_sort, "bubble_sort");
}
