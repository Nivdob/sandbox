#pragma once

#include <vector>

bool test_sort(void (*f)(std::vector<int> *v), const char *const name);
int test_sort_main(int argc, char *argv[],
				   void (*f)(std::vector<int> *v), const char *const name);
