#pragma once

#include <vector>

void array_read(FILE *const f, std::vector<int> *const v);
void array_read(const char *const name, std::vector<int> *const v);
void array_write(FILE *const f, const std::vector<int> *const v);
void array_write(const char *const name, const std::vector<int> *const v);
