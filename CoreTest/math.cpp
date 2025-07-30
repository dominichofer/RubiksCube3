#include "pch.h"
#include <random>

TEST(binomial, small_values)
{
	EXPECT_EQ(binomial(0, 0), 1);

	EXPECT_EQ(binomial(1, 0), 1);
	EXPECT_EQ(binomial(1, 1), 1);

	EXPECT_EQ(binomial(2, 0), 1);
	EXPECT_EQ(binomial(2, 1), 2);
	EXPECT_EQ(binomial(2, 2), 1);

	EXPECT_EQ(binomial(3, 0), 1);
	EXPECT_EQ(binomial(3, 1), 3);
	EXPECT_EQ(binomial(3, 2), 3);
	EXPECT_EQ(binomial(3, 3), 1);

	EXPECT_EQ(binomial(4, 0), 1);
	EXPECT_EQ(binomial(4, 1), 4);
	EXPECT_EQ(binomial(4, 2), 6);
	EXPECT_EQ(binomial(4, 3), 4);
	EXPECT_EQ(binomial(4, 4), 1);
}

TEST(combination_index, small_values)
{
	EXPECT_EQ(combination_index(1, std::array{ 0 }), 0);

	EXPECT_EQ(combination_index(2, std::array{ 0 }), 0);
	EXPECT_EQ(combination_index(2, std::array{ 1 }), 1);
	EXPECT_EQ(combination_index(2, std::array{ 0, 1 }), 0);

	EXPECT_EQ(combination_index(3, std::array{ 0 }), 0);
	EXPECT_EQ(combination_index(3, std::array{ 1 }), 1);
	EXPECT_EQ(combination_index(3, std::array{ 2 }), 2);

	EXPECT_EQ(combination_index(3, std::array{ 0, 1 }), 0);
	EXPECT_EQ(combination_index(3, std::array{ 0, 2 }), 1);
	EXPECT_EQ(combination_index(3, std::array{ 1, 2 }), 2);

	EXPECT_EQ(combination_index(3, std::array{ 0, 1, 2 }), 0);

	EXPECT_EQ(combination_index(4, std::array{ 0 }), 0);
	EXPECT_EQ(combination_index(4, std::array{ 1 }), 1);
	EXPECT_EQ(combination_index(4, std::array{ 2 }), 2);
	EXPECT_EQ(combination_index(4, std::array{ 3 }), 3);

	EXPECT_EQ(combination_index(4, std::array{ 0, 1 }), 0);
	EXPECT_EQ(combination_index(4, std::array{ 0, 2 }), 1);
	EXPECT_EQ(combination_index(4, std::array{ 0, 3 }), 2);
	EXPECT_EQ(combination_index(4, std::array{ 1, 2 }), 3);
	EXPECT_EQ(combination_index(4, std::array{ 1, 3 }), 4);
	EXPECT_EQ(combination_index(4, std::array{ 2, 3 }), 5);
}

std::vector<int> nth_combination(int64_t n, int64_t k, int64_t index)
{
	std::vector<int> combination(k);
	nth_combination(n, k, index, combination);
	return combination;
}

TEST(nth_combination, small_values)
{
	// 2 choose 1
	EXPECT_EQ(nth_combination(2, 1, 0), (std::vector<int>{0}));
	EXPECT_EQ(nth_combination(2, 1, 1), (std::vector<int>{1}));

	// 2 choose 2
	EXPECT_EQ(nth_combination(2, 2, 0), (std::vector<int>{0, 1}));

	// 3 choose 1
	EXPECT_EQ(nth_combination(3, 1, 0), (std::vector<int>{0}));
	EXPECT_EQ(nth_combination(3, 1, 1), (std::vector<int>{1}));
	EXPECT_EQ(nth_combination(3, 1, 2), (std::vector<int>{2}));

	// 3 choose 2
	EXPECT_EQ(nth_combination(3, 2, 0), (std::vector<int>{0, 1}));
	EXPECT_EQ(nth_combination(3, 2, 1), (std::vector<int>{0, 2}));
	EXPECT_EQ(nth_combination(3, 2, 2), (std::vector<int>{1, 2}));

	// 3 choose 3
	EXPECT_EQ(nth_combination(3, 3, 0), (std::vector<int>{0, 1, 2}));

	// 4 choose 1
	EXPECT_EQ(nth_combination(4, 1, 0), (std::vector<int>{0}));
	EXPECT_EQ(nth_combination(4, 1, 1), (std::vector<int>{1}));
	EXPECT_EQ(nth_combination(4, 1, 2), (std::vector<int>{2}));
	EXPECT_EQ(nth_combination(4, 1, 3), (std::vector<int>{3}));

	// 4 choose 2
	EXPECT_EQ(nth_combination(4, 2, 0), (std::vector<int>{0, 1}));
	EXPECT_EQ(nth_combination(4, 2, 1), (std::vector<int>{0, 2}));
	EXPECT_EQ(nth_combination(4, 2, 2), (std::vector<int>{0, 3}));
	EXPECT_EQ(nth_combination(4, 2, 3), (std::vector<int>{1, 2}));
	EXPECT_EQ(nth_combination(4, 2, 4), (std::vector<int>{1, 3}));
	EXPECT_EQ(nth_combination(4, 2, 5), (std::vector<int>{2, 3}));

	// 4 choose 3
	EXPECT_EQ(nth_combination(4, 3, 0), (std::vector<int>{0, 1, 2}));
	EXPECT_EQ(nth_combination(4, 3, 1), (std::vector<int>{0, 1, 3}));
	EXPECT_EQ(nth_combination(4, 3, 2), (std::vector<int>{0, 2, 3}));
	EXPECT_EQ(nth_combination(4, 3, 3), (std::vector<int>{1, 2, 3}));

	// 4 choose 4
	EXPECT_EQ(nth_combination(4, 4, 0), (std::vector<int>{0, 1, 2, 3}));
}

// Tests that nth_combination is the inverse of combination_index.
// Fuzz test.
TEST(nth_combination, is_inverse_of_combination_index)
{
	std::mt19937 rnd{ 1233 }; // Fixed seed for reproducibility.
	for (int i = 0; i < 100'000; ++i)
	{
		int n = std::uniform_int_distribution<int>{ 1, 50 }(rnd);
		int k = std::uniform_int_distribution<int>{ 1, n }(rnd);
		int64_t index = std::uniform_int_distribution<int64_t>{ 0, binomial(n, k) - 1 }(rnd);
		auto combination = nth_combination(n, k, index);
		EXPECT_EQ(combination_index(n, combination), index);
	}
}

TEST(factorial, recursive)
{
	EXPECT_EQ(factorial(0), 1);
	for (int i = 1; i <= 20; i++)
		EXPECT_EQ(factorial(i), factorial(i - 1) * i);
}

TEST(is_even_permutation, of_range)
{
	EXPECT_TRUE(is_even_permutation(std::vector<int>{0, 1, 2}));
	EXPECT_TRUE(is_odd_permutation(std::vector<int>{0, 2, 1}));
	EXPECT_TRUE(is_even_permutation(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
	EXPECT_TRUE(is_even_permutation(std::vector<int>{0, 1, 3, 2, 4, 5, 6, 8, 7, 9, 10}));
}

TEST(is_even_permutation, of_lexicographical_index)
{
	std::mt19937 rnd{ 2349873 };
	std::vector<int> p(20);
	for (int i = 0; i < 1'000'000; ++i)
	{
		std::ranges::iota(p, 0);
		std::ranges::shuffle(p, rnd);
		EXPECT_EQ(is_even_permutation(p), is_even_permutation(permutation_index(p)));
	}
}

template <typename... T>
int64_t permutation_index(T... args)
{
	return permutation_index(std::array{ args... });
}

TEST(permutation_index, size_1)
{
	EXPECT_EQ(permutation_index(0), 0);
}

TEST(permutation_index, size_2)
{
	EXPECT_EQ(permutation_index(0, 1), 0);
	EXPECT_EQ(permutation_index(1, 0), 1);
}

TEST(permutation_index, size_3)
{
	EXPECT_EQ(permutation_index(0, 1, 2), 0);
	EXPECT_EQ(permutation_index(0, 2, 1), 1);
	EXPECT_EQ(permutation_index(1, 0, 2), 2);
	EXPECT_EQ(permutation_index(1, 2, 0), 3);
	EXPECT_EQ(permutation_index(2, 0, 1), 4);
	EXPECT_EQ(permutation_index(2, 1, 0), 5);
}

TEST(permutation_index, size_4)
{
	EXPECT_EQ(permutation_index(0, 1, 2, 3), 0);
	EXPECT_EQ(permutation_index(0, 1, 3, 2), 1);
	EXPECT_EQ(permutation_index(0, 2, 1, 3), 2);
	EXPECT_EQ(permutation_index(0, 2, 3, 1), 3);
	EXPECT_EQ(permutation_index(0, 3, 1, 2), 4);
	EXPECT_EQ(permutation_index(0, 3, 2, 1), 5);
	EXPECT_EQ(permutation_index(1, 0, 2, 3), 6);
	EXPECT_EQ(permutation_index(1, 0, 3, 2), 7);
	EXPECT_EQ(permutation_index(1, 2, 0, 3), 8);
	EXPECT_EQ(permutation_index(1, 2, 3, 0), 9);
	EXPECT_EQ(permutation_index(1, 3, 0, 2), 10);
	EXPECT_EQ(permutation_index(1, 3, 2, 0), 11);
	EXPECT_EQ(permutation_index(2, 0, 1, 3), 12);
	EXPECT_EQ(permutation_index(2, 0, 3, 1), 13);
	EXPECT_EQ(permutation_index(2, 1, 0, 3), 14);
	EXPECT_EQ(permutation_index(2, 1, 3, 0), 15);
	EXPECT_EQ(permutation_index(2, 3, 0, 1), 16);
	EXPECT_EQ(permutation_index(2, 3, 1, 0), 17);
	EXPECT_EQ(permutation_index(3, 0, 1, 2), 18);
	EXPECT_EQ(permutation_index(3, 0, 2, 1), 19);
	EXPECT_EQ(permutation_index(3, 1, 0, 2), 20);
	EXPECT_EQ(permutation_index(3, 1, 2, 0), 21);
	EXPECT_EQ(permutation_index(3, 2, 0, 1), 22);
	EXPECT_EQ(permutation_index(3, 2, 1, 0), 23);
}

TEST(nth_permutation, small_sets)
{
	for (int size = 0; size < 9; ++size)
	{
		std::vector<int> p(size), q(size);
		std::ranges::iota(p, 0);
		for (int64_t i = 0; i < factorial(size); ++i)
		{
			nth_permutation(i, q.begin(), size);
			EXPECT_EQ(q, p);
			std::ranges::next_permutation(p);
		}
	}
}

// Tests that nth_permutation is the inverse of permutation_index.
// Fuzz test
TEST(nth_permutation, is_inverse_of_permutation_index)
{
	std::mt19937 rnd{ 1233 }; // Fixed seed for reproducibility.
	std::array<int, 20> p, q;
	std::ranges::iota(p, 0);
	for (int i = 0; i < 10'000; ++i)
	{
		std::ranges::shuffle(p, rnd);
		nth_permutation(permutation_index(p), q);
		for (int j = 0; j < 20; ++j)
			EXPECT_EQ(p[j], q[j]);
	}
}

// Tests that permutation_index/2 is a bijection,
// between even permutations and [0, factorial(n)/2);
// and between odd permutations and [0, factorial(n)/2).
TEST(permutation_index_half, is_bijection)
{
	for (int size = 0; size < 11; ++size)
	{
		std::vector<int> p(size);
		int even_permutations = 0;
		int odd_permutations = 0;
		for (int i = 0; i < factorial(size); ++i)
		{
			nth_permutation(i, p);
			if (is_even_permutation(p))
			{
				EXPECT_EQ(permutation_index(p) / 2, even_permutations);
				++even_permutations;
			}
			else
			{
				EXPECT_EQ(permutation_index(p) / 2, odd_permutations);
				++odd_permutations;
			}
		}
	}
}
