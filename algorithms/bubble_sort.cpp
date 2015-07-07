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

	void read_file(const char *const name, std::vector<int> *const v)
	{
		FILE *const f = fopen(name, "r");
		if (0 != f)
		{
			read_file(f, v);
			fclose(f);
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
			for (size_t j = i; 0 < j-- && u[j] > x;)
			{
				u[j + 1] = u[j];
				u[j] = x;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	std::vector<int> v;
	if (1 < argc)
	{
		read_file(argv[1], &v);
	}
	else
	{
		read_file(stdin, &v);
	}
	//bubble_sort(&v);
	insertion_sort(&v);
	write_file(stderr, &v);
	return 0;
}
