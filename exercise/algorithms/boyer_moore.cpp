#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <stddef.h>
#include <stdio.h>
#include <countof.h>
#include <array_file.h>

namespace
{
	/* Returns pointer on largest value that is smaller than v (or 0 aka null).
	 * [b, e) array must be sorted, so larger values have larger indexies.
	 */
	const size_t *prev_pos(const size_t *const b, const size_t *const e,
						   const size_t v)
	{
		const size_t *jb = b;
		const size_t *je = e;
		ptrdiff_t dj = je - jb;
		/* For small arrays just use linear search. */
		if (8 >= dj)
		{
			for (const size_t *j = je; jb < j--;)
			{
				if (*j < v)
				{
					return j;
				}
			}
			return 0;
		}
		/* For larger arrays use binary search. */
		assert(8 < dj);
		const size_t *p = 0;
		do
		{
			const size_t *const j = jb + dj / 2;
			if (*j < v)
			{
				p = j;
				jb = j + 1;
			}
			else
			{
				je  = j;
			}
			dj = je - jb;
		}
		while (0 != dj);
		return p;
	}

	/* Returns number of characters matched.
	 */
	size_t sfx_match(const char *const sb, const char *const se,
					 const char *const pe)
	{
		assert(sb <= pe && pe <= se);
		for (const char *sl = pe, *sr = se; sb < sl;)
		{
			--sl; --sr;
			if (*sl != *sr)
			{
				return pe - sl - 1;
			}
		}
		return pe - sb;
	}

	/* Compute fundamental preprocessing N[j] (which is a reverse of Z[j]).
	 * N[j] is the length of the longest string that has its right end at
	 * position j and is a suffix of s.
	 */
	void compute_Nj(const char *const s, const size_t n,
					size_t *const Nj)
	{
		if (0 == n)
		{
			return;
		}
		/* String is a suffix of itself. */
		Nj[n - 1] = n;
		if (1 == n)
		{
			return;
		}
		assert(1 < n);
		const char *const se = s + n;
		size_t *const Nje = Nj + n;
		size_t *const Nj2 = Nje - 2;
		/* Find first missmatch for j = n - 2 (suffix shift is 1). */
		*Nj2 = n - 1;
		for (const char *sl = se - 1, *sr; s < (sr = sl--);)
		{
			if (*sl != *sr)
			{
				*Nj2 = se - sr - 1;
				break;
			}
		}
		/* If Nj[n - 2] is not 0, then we have Nj[n - 2] + 1 characters that are
		 * the same (well, even when Nj[n - 2] is 0, then we still have 0 + 1
		 * characters that are the same - it's just a single character).
		 * A small optimization to handle that case.
		 */
		size_t *k = Nj2;
		for (size_t *const kb = Nje - (*Nj2 + 1), *kp; kb < (kp = k);)
		{

			*(--k) = *kp - 1;
		}
		/* Main loop. */
		size_t *l = Nje;
		size_t *r = Nje;
		for (size_t *kp; Nj < (kp = k--);)
		{
			const size_t i = k - Nj;
			if (l <= k)
			{
				const size_t *const k_prime = Nje - (r - k);
				const size_t b = k - l + 1;
				if (*k_prime < b)
				{
					*k = *k_prime;
				}
				else
				{
					const size_t len = sfx_match(s, se - b, s + (l - Nj));
					*k = b + len;
				}
			}
			else
			{
				const size_t len = Nj[i] = sfx_match(s, se, s + i + 1);
				if (0 < len)
				{
					l = k - len + 1;
					r = k + 1;
				}
			}
		}
	}

	/* L'[i] - largest index j less than n such that N[j] = n - i. Or in other
	 * words, right end position of the rightmost substring that is equal to the
	 * suffix i..n AND symbol to the left end of that string is not equal to
	 * symbol at i - 1. This last AND is the difference between L'[i] and L[i]
	 * (the later doesn't have that AND).
	 */
	void compute_Li_prime(const size_t *const Nj, const size_t n,
						  size_t *const Li_prime)
	{
		if (0 == n)
		{
			return;
		}
		memset(Li_prime, 0, sizeof(*Li_prime) * n);
		const size_t m = n - 1;
		for (size_t i = 0; m > i; ++i)
		{
			const size_t k = Nj[i];
			if (0 < k)
			{
				Li_prime[n - k] = i;
			}
		}
	}

	void compute_li_prime(const size_t *const Nj, const size_t n,
						  size_t *const li_prime)
	{
		if (0 == n)
		{
			return;
		}
		const size_t m = n - 1;
		const size_t *k = Nj;
		size_t *h = li_prime + n;
		size_t v = *--h = *k++;
		for (size_t i = 1; m > i++; ++k)
		{
			v = *k == i? *k: v;
			*--h = v;
		}
		*--h = v;
	}

	/*
	void compute_Li(const size_t *const Li_prime, const size_t n,
					size_t *const Li)
	{
		Li[0] = Li_prime[0];
		Li[1] = Li_prime[1];
		for (size_t i = 2; n > i; ++i)
		{
			Li[i] = std::max(Li[i - 1], Li_prime[i]);
		}
	}
	*/

	class bm_pattern
	{
	public:
		bm_pattern(const char *const p, const size_t p_sz);
		size_t bad_character(const size_t i, const char ch) const;
		size_t good_suffix(const size_t i) const;
	private:
		/* For each char (as index) contains end of its range in m_bc_pos. */
		size_t m_bc_range[2 << 8 * sizeof(char)];
		/* Contains indexies in the pattern for every char value. Layout for
		 * "abcabcda":
		 *   {0,3,7,1,4,2,5,6} - m_bc_pos values
		 *   |aaaaa|bbb|ccc|d| - corresponding characters
		 *   [0,1,2,3,4,5,6,7] - array indexies
		 * In that case:
		 *   m_bc_pos['a'] = 3
		 *   m_bc_pos['b'] = 5
		 *   m_bc_pos['c'] = 7
		 *   m_bc_pos['d'] = 8
		 */
		std::vector<size_t> m_bc_pos;
	};

	bm_pattern::bm_pattern(const char *const p, const size_t p_sz)
	{
		/* bad character rule (in its "extended" version) */
		size_t bc_count[_countof(m_bc_range)] = {0};
		for (size_t i = 0; p_sz > i; ++i)
		{
			++bc_count[(size_t)p[i]];
		}
		for (size_t i = 0, range = 0; _countof(m_bc_range) > i; ++i)
		{
			range += bc_count[i];
			m_bc_range[i] = range;
		}
		m_bc_pos.resize(p_sz);
		for (size_t i = 0; p_sz > i; ++i)
		{
			const size_t k = (size_t)p[i];
			const size_t j = m_bc_range[k] - bc_count[k]--;
			m_bc_pos[j] = i;
		}
		/* good suffix rule */
	}

	size_t bm_pattern::bad_character(const size_t i, const char ch) const
	{
		const size_t k = (size_t)ch;
		const size_t jb = 0 == k? 0: m_bc_range[k - 1];
		const size_t je = m_bc_range[k];
		const size_t *const p = prev_pos(&m_bc_pos[jb], &m_bc_pos[je], i);
		if (0 != p)
		{
			return i - *p;
		}
		return i + 1;
	}

	size_t bm_pattern::good_suffix(const size_t i) const
	{
		/*
		size_t step = i + 1;
		const char *const sfx = p + i + 1;
		const size_t sfx_sz = p_sz - i - 1;
		for (size_t j = i; 1 < j--;)
		{
			if (0 == memcmp(sfx, p + j, sfx_sz) && p[i] != p[j - 1])
			{
				fprintf(stderr, "    sfx match: i=%zu, j=%zu, sfx_sz=%zu\n",
						i, j, sfx_sz);
				step -= j;
				break;
			}
		}
		fprintf(stderr, "    good_suffix: step %zu\n", step);
		return step;
		*/
		(void)i;
		return 0;
	}

	std::vector<size_t> boyer_moore(const char *const t, const size_t t_sz,
									const char *const p, const size_t p_sz)
	{
		std::vector<size_t> indexies;
		const bm_pattern pt(p, p_sz);
		for (size_t ae = p_sz; t_sz >= ae;)
		{
			fprintf(stderr, "---- ---- ---- ----\n");
			fprintf(stderr, "> \"%s\"\n> ", t);
			for (int i = 0; int(ae - p_sz) > i; ++i) fprintf(stderr, " ");
			fprintf(stderr, "\"%s\"\n", p);
			size_t step = 1;
			bool match = true;
			for (size_t i = ae, j = p_sz; --i, 0 < j--;)
			{
				if (t[i] != p[j])
				{
					const size_t step_bc = pt.bad_character(j, t[i]);
					const size_t step_gs = pt.good_suffix(j);
					step = std::max(step, step_bc);
					step = std::max(step, step_gs);
					match = false;
					break;
				}
			}
			if (match)
			{
				const size_t k = ae - p_sz;
				fprintf(stderr, "match at %zu\n", k);
				indexies.push_back(k);
			}
			ae += step;
		}
		return indexies;
	}

	std::vector<size_t> boyer_moore_findall(const std::string &t,
											const std::string &p)
	{
		return boyer_moore(t.c_str(), t.size(), p.c_str(), p.size());
	}

	struct sfx_match_test_case
	{
		const std::string s;
		const size_t p;
		const size_t len;
	};

	const sfx_match_test_case c_sfx_match_test_cases[] =
	{
		{"", 0, 0},
		{"a", 0, 0},
		{"a", 1, 1},
		{"aa", 0, 0},
		{"aa", 1, 1},
		{"aa", 2, 2},
		{"aaa", 0, 0},
		{"aaa", 1, 1},
		{"aaa", 2, 2},
		{"aaa", 3, 3},
		{"banana", 0, 0},
		{"banana", 1, 0},
		{"banana", 2, 1},
		{"banana", 3, 0},
		{"banana", 4, 3},
		{"banana", 5, 0},
		{"banana", 6, 6},
	};

	bool test_sfx_match()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_sfx_match_test_cases) > k; ++k)
		{
			const sfx_match_test_case &tc = c_sfx_match_test_cases[k];
			const char *const s = tc.s.c_str();
			const size_t len = sfx_match(s, s + tc.s.size(), s + tc.p);
			if (len != tc.len)
			{
				fprintf(stderr, "sfx_match test case #%zu failed\n", k);
				fprintf(stderr, "\tString: \"%s\"\n", s);
				fprintf(stderr, "\tPosition: %zu\n", tc.p);
				fprintf(stderr, "\tExpected: %zu\n", tc.len);
				fprintf(stderr, "\tActual: %zu\n", len);
				ok = false;
			}
		}
		return ok;
	}

	struct prev_pos_test_case
	{
		const std::vector<size_t> v;
		const size_t i;
		const int p;
	};

	const prev_pos_test_case c_prev_pos_test_cases[] =
	{
		{{0,1,2,3,4,5,6,7,8,9}, 9,  8},
		{{0,1,2,3,4,5,6,7,8,9}, 8,  7},
		{{0,1,2,3,4,5,6,7,8,9}, 7,  6},
		{{0,1,2,3,4,5,6,7,8,9}, 6,  5},
		{{0,1,2,3,4,5,6,7,8,9}, 5,  4},
		{{0,1,2,3,4,5,6,7,8,9}, 4,  3},
		{{0,1,2,3,4,5,6,7,8,9}, 3,  2},
		{{0,1,2,3,4,5,6,7,8,9}, 2,  1},
		{{0,1,2,3,4,5,6,7,8,9}, 1,  0},
		{{0,1,2,3,4,5,6,7,8,9}, 0, -1},
		{{0,0,2,3,3,3,4,4,4,5}, 5,  8},
		{{0,0,2,3,3,3,4,4,4,5}, 4,  5},
		{{0,0,2,3,3,3,4,4,4,5}, 3,  2},
		{{0,0,2,3,3,3,4,4,4,5}, 2,  1},
		{{0,0,2,3,3,3,4,4,4,5}, 1,  1},
		{{0,0,2,3,3,3,4,4,4,5}, 0, -1},
		{{0,1,2,3,4}, 4, 3},
		{{0,1,2,3,4}, 3, 2},
		{{0,1,2,3,4}, 2, 1},
		{{0,1,2,3,4}, 1, 0},
		{{0,1,2,3,4}, 0, -1},
	};

	bool test_prev_pos()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_prev_pos_test_cases) > k; ++k)
		{
			const prev_pos_test_case &tc = c_prev_pos_test_cases[k];
			const size_t *const v = &tc.v[0];
			const size_t *const p = prev_pos(v, v + tc.v.size(), tc.i);
			const size_t *const exp = -1 == tc.p? 0: v + tc.p;
			if (p != exp)
			{
				fprintf(stderr, "insert_pos test case #%zu failed\n", k);
				fprintf(stderr, "\tArray:\n\t");
				array_write(stderr, &tc.v);
				fprintf(stderr, "\tValue: %zu\n", tc.i);
				fprintf(stderr, "\tExpected: %i\n", tc.p);
				fprintf(stderr, "\tActual: %i\n", 0 == p? -1: (int)(p - v));
				ok = false;
			}
		}
		return ok;
	}

	struct compute_Nj_test_case
	{
		const std::string s;
		const std::vector<size_t> Nj;
	};

	const compute_Nj_test_case c_compute_Nj_test_cases[] =
	{
		{"", {}},
		{"a", {1}},
		{"aa", {1, 2}},
		{"aaa", {1, 2, 3}},
		{"aaaa", {1, 2, 3, 4}},
		{"aaaaa", {1, 2, 3, 4, 5}},
		{"ab", {0, 2}},
		{"aab", {0, 0, 3}},
		{"aaab", {0, 0, 0, 4}},
		{"aaaab", {0, 0, 0, 0, 5}},
		{"abb", {0, 1, 3}},
		{"abc", {0, 0, 3}},
		{"cbaaaa", {0, 0, 1, 2, 3, 6}},
		{"infinity", {0, 0, 0, 0, 0, 0, 0, 8}},
		{"banana", {0, 1, 0, 3, 0, 6}},
		{"abcabcabc", {0, 0, 3, 0, 0, 6, 0, 0, 9}},
		{"cabababcabab", {0, 0, 2, 0, 5, 0, 4, 0, 0, 2, 0, 12}},
	};

	bool test_compute_Nj()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_compute_Nj_test_cases) > k; ++k)
		{
			const compute_Nj_test_case &tc = c_compute_Nj_test_cases[k];
			std::vector<size_t> Nj(tc.s.size());
			compute_Nj(tc.s.c_str(), tc.s.size(), &Nj[0]);
			if (Nj != tc.Nj)
			{
				fprintf(stderr, "compute_Nj test case #%zu failed\n", k);
				fprintf(stderr, "\tString: \"%s\"\n", tc.s.c_str());
				fprintf(stderr, "\tExpected:\n\t");
				array_write(stderr, &tc.Nj);
				fprintf(stderr, "\tActual:\n\t");
				array_write(stderr, &Nj);
				ok = false;
			}
		}
		return ok;
	}

	void naive_compute_Nj(const char * const s, const size_t n,
						  size_t * const Nj)
	{
		const char *const se = s + n;
		const char *sj = se;
		for (size_t *k = Nj + n; Nj < k--; --sj)
		{
			*k = sfx_match(s, se, sj);
		}
	}

	bool test_compute_Nj_vs_naive()
	{
		bool ok = true;
		const size_t max_len = 8;
		for (size_t s_len = 0; max_len > s_len; ++s_len)
		{
			std::vector<size_t> Nj_1(s_len);
			std::vector<size_t> Nj_2(s_len);
			const char alph_b = 'a';
			const char alph_e = alph_b + s_len;
			std::string s(s_len, alph_b);
			for (;;)
			{
				naive_compute_Nj(s.c_str(), s.size(), &Nj_1[0]);
				compute_Nj(s.c_str(), s.size(), &Nj_2[0]);
				if (Nj_1 != Nj_2)
				{
					fprintf(stderr, "compute_Nj vs naive test case failed\n");
					fprintf(stderr, "\tString: \"%s\"\n", s.c_str());
					fprintf(stderr, "\tExpected:\n\t");
					array_write(stderr, &Nj_1);
					fprintf(stderr, "\tActual:\n\t");
					array_write(stderr, &Nj_2);
					ok = false;
				}
				size_t k = 0;
				while (s_len != k)
				{
					if (alph_e != ++s[k])
					{
						k = 0;
						break;
					}
					s[k++] = alph_b;
				}
				if (s_len == k)
				{
					break;
				}
			}
		}
		return ok;
	}

	struct compute_Li_prime_test_case
	{
		const std::string s;
		const std::vector<size_t> Li_prime;
	};

	const compute_Li_prime_test_case c_compute_Li_prime_test_cases[] =
	{
		{"abb", {0, 0, 1}},
		{"abracadabra", {0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 7}},
		{"GGTAGGT", {0, 0, 0, 0, 2, 0, 0}},
	};

	bool test_compute_Li_prime()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_compute_Li_prime_test_cases) > k; ++k)
		{
			const compute_Li_prime_test_case &tc = c_compute_Li_prime_test_cases[k];
			std::vector<size_t> Nj(tc.s.size());
			compute_Nj(tc.s.c_str(), tc.s.size(), &Nj[0]);
			std::vector<size_t> Li_prime(tc.s.size());
			compute_Li_prime(&Nj[0], Nj.size(), &Li_prime[0]);
			bool check = true;
			for (size_t i = 0; Li_prime.size() > i; ++i)
			{
				check |= Nj[Li_prime[i]] == Li_prime.size() - i;
			}
			if (Li_prime != tc.Li_prime || !check)
			{
				fprintf(stderr, "compute_Li_prime test case #%zu failed\n", k);
				fprintf(stderr, "\tString: \"%s\"\n", tc.s.c_str());
				fprintf(stderr, "\tExpected:\n\t");
				array_write(stderr, &tc.Li_prime);
				fprintf(stderr, "\tActual:\n\t");
				array_write(stderr, &Li_prime);
				fprintf(stderr, "\tNj:\n\t");
				array_write(stderr, &Nj);
				ok = false;
			}
		}
		return ok;
	}

	struct compute_li_prime_test_case
	{
		const std::string s;
		const std::vector<size_t> li_prime;
	};

	const compute_li_prime_test_case c_compute_li_prime_test_cases[] =
	{
		{"", {}},
		{"a", {1}},
		{"ab", {0, 0}},
		{"aa", {1, 1}},
		{"aaa", {2, 2, 1}},
		{"abracadabra", {4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1}},
	};

	bool test_compute_li_prime()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_compute_li_prime_test_cases) > k; ++k)
		{
			const compute_li_prime_test_case &tc = c_compute_li_prime_test_cases[k];
			std::vector<size_t> Nj(tc.s.size());
			compute_Nj(tc.s.c_str(), tc.s.size(), &Nj[0]);
			std::vector<size_t> li_prime(tc.s.size());
			compute_li_prime(&Nj[0], Nj.size(), &li_prime[0]);
			if (li_prime != tc.li_prime)
			{
				fprintf(stderr, "compute_li_prime test case #%zu failed\n", k);
				fprintf(stderr, "\tString: \"%s\"\n", tc.s.c_str());
				fprintf(stderr, "\tExpected:\n\t");
				array_write(stderr, &tc.li_prime);
				fprintf(stderr, "\tActual:\n\t");
				array_write(stderr, &li_prime);
				fprintf(stderr, "\tNj:\n\t");
				array_write(stderr, &Nj);
				ok = false;
			}
		}
		return ok;
	}

	struct bad_character_test_case
	{
		const std::string pattern;
		const size_t missmatch_i;
		const char missmatch_ch;
		const size_t step;
	};

	const bad_character_test_case c_bad_character_test_cases[] =
	{
		{"infinity", 6, 'n', 2},
		{"infinity", 6, 'x', 7},
		{"bbaacab", 3, 'b', 2},
	};

	struct test_case
	{
		const std::string text;
		const std::string pattern;
		const std::vector<size_t> indexies;
	};

	const test_case c_test_cases[] =
	{
		{"", "", {0}},
		{"", "1", {}},
		{"1", "1", {0}},
		{"123", "2", {1}},
		{"12345445", "445", {5}},
	};

	bool test_bad_character()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_bad_character_test_cases) > k; ++k)
		{
			const bad_character_test_case &tc = c_bad_character_test_cases[k];
			const bm_pattern pt(tc.pattern.c_str(), tc.pattern.size());
			const size_t step = pt.bad_character(tc.missmatch_i, tc.missmatch_ch);
			if (step != tc.step)
			{
				std::fprintf(stderr, "bad_character test case #%zu failed\n", k);
				std::fprintf(stderr, "\tPattern:   %s\n", tc.pattern.c_str());
				std::string pad(tc.missmatch_i, ' ');
				std::fprintf(stderr, "\tMissmatch: %s%c\n",
							 pad.c_str(), tc.missmatch_ch);
				std::fprintf(stderr, "\tExpected step: %zu\n", tc.step);
				std::fprintf(stderr, "\tActual step: %zu\n", step);
				ok = false;
			}
		}
		return ok;
	}

	bool test_search()
	{
		bool ok = true;
		for (size_t k = 0; _countof(c_test_cases) > k; ++k)
		{
			const test_case &tc = c_test_cases[k];
			const std::vector<size_t> indexies =
					boyer_moore_findall(tc.text, tc.pattern);
			if (indexies != tc.indexies)
			{
				std::fprintf(stderr, "test case #%zu failed\n", k);
				std::fprintf(stderr, "\tText:\n\t\"%s\"\n", tc.text.c_str());
				std::fprintf(stderr, "\tPattern:\n\t\"%s\"\n", tc.pattern.c_str());
				std::fprintf(stderr, "\tExpected indexies:\n\t");
				array_write(stderr, &tc.indexies);
				std::fprintf(stderr, "\tFound indexies:\n\t");
				array_write(stderr, &indexies);
				ok = false;
			}
		}
		return ok;
	}
}

int main(int argc, char *argv[])
{
	if (3 > argc)
	{
		bool ok = true;
		ok |= test_prev_pos();
		ok |= test_sfx_match();
		ok |= test_compute_Nj();
		ok |= test_compute_Nj_vs_naive();
		ok |= test_compute_Li_prime();
		ok |= test_compute_li_prime();
		ok |= test_bad_character();
		ok |= test_search();
		return ok? 0: -1;
	}
	const char *const t = argv[1];
	const char *const p = argv[2];
	boyer_moore(t, strlen(t), p, strlen(p));
	return 0;
}

