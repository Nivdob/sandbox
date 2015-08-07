#include <cstdlib>
#include "array_file.h"

void array_read(FILE *const f, std::vector<int> *const v)
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

void array_read(const char *const name, std::vector<int> *const v)
{
	FILE *const f = fopen(name, "r");
	if (0 != f)
	{
		array_read(f, v);
		fclose(f);
	}
}

void array_write(FILE *const f, const std::vector<int> *const v)
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

void array_write(const char *const name, const std::vector<int> *const v)
{
	FILE *const f = fopen(name, "w");
	if (0 != f)
	{
		array_write(f, v);
		fclose(f);
	}
}
