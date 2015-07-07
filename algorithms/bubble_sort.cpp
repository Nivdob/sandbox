#include <cstdlib>
#include <algorithm>
#include <vector>

namespace
{
	void read_file(FILE *const f, std::vector<int> *const v)
	{
		int ch;
		char buf[16];
		char *p = buf;
		const char *const e = buf + sizeof(buf) - 1;
		for (;;)
		{
			ch = fgetc(f);
			if (!isnumber(ch))
			{
				if (p != buf)
				{
					*p = 0;
					v->push_back(std::strtol(buf, &p, 10));
					p = buf;
				}
				if (EOF == ch)
				{
					break;
				}
			}
			else if (p != e)
			{
				*p++ = ch;
			}
		}
	}

	void write_file(FILE *const f, const std::vector<int> *const v)
	{
		if (!v->empty())
		{
			fprintf(f, "%i", *v->begin());
			std::for_each(v->begin() + 1, v->end(), [f](const int x)
			{
				fprintf(f, ", %i", x);
			});
		}
		fprintf(f, "\n");
	}

	void bubble_sort(std::vector<int> *const v)
	{
		const auto b = v->begin();
		auto e = v->end();
		for(bool sorted = v->empty(); !sorted; --e)
		{
			sorted = true;
			for (auto i = b, j = b + 1; e != j; i = j++)
			{
				if (*i > *j)
				{
					std::swap(*i, *j);
					sorted = false;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;
	FILE *const f = fopen(argv[1], "r");
	std::vector<int> v;
	read_file(f, &v);
	fclose(f);
	bubble_sort(&v);
	write_file(stderr, &v);
	return 0;
}
