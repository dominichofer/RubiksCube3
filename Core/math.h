#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <ranges>
#include <stdexcept>

namespace
{
	constexpr std::array<std::array<int64_t, 13>, 13> make_binomial_lookup() {
		std::array<std::array<int64_t, 13>, 13> table{};
		for (int64_t n = 0; n <= 12; ++n) {
			table[n][0] = 1;
			for (int64_t k = 1; k <= n; ++k) {
				table[n][k] = table[n - 1][k - 1] + table[n - 1][k];
			}
		}
		return table;
	}

	// constexpr lookup table (evaluated at compile time)
	constexpr static auto binomial_lookup = make_binomial_lookup();
}

// Binomial coefficient "n choose k".
constexpr int64_t binomial(int64_t n, int64_t k)
{
	if (k > n)
		return 0;
	if (n <= 12)
		return binomial_lookup[n][k];
	if (k == 0 || k == n)
		return 1;
	if (k > n - k)
		k = n - k;

	int64_t result = 1;
	for (int64_t i = 0; i < k; ++i)
	{
		if ((n - i) % (i + 1) == 0)
			result *= (n - i) / (i + 1);
		else if (result % (i + 1) == 0)
			result = result / (i + 1) * (n - i);
		else
			result = result * (n - i) / (i + 1);
	}
	return result;
}

// Returns the index of the combination
// in the lexicographically sorted list of all possible
// combinations of n elements taken k at a time.
template <std::ranges::random_access_range R>
constexpr int64_t combination_index(int64_t n, const R& combination)
{
	int64_t index = 0;
	int64_t j = 0;
	int64_t k = std::ranges::size(combination);
	for (int64_t i = 0; i < k; ++i)
		for (++j; j < combination[i] + 1; ++j)
			index += binomial(n - j, k - i - 1);
	return index;
}

template <std::ranges::random_access_range R>
void nth_combination(int64_t n, int64_t k, int64_t index, R& combination)
{
	if (k < 1 or k > n)
		throw std::out_of_range("Invalid combination size");
	if (k > std::ranges::size(combination))
		throw std::out_of_range("Combination size exceeds output range size");

	int64_t size = 0;
	for (int i = 0; i < n; i++)
	{
		int64_t count = binomial(n - 1 - i, k - size - 1);
		if (count > index)
		{
			combination[size++] = i;
			if (size == k)
				return;
		}
		else
			index -= count;
	}
}

constexpr int64_t factorial(int64_t n)
{
	constexpr int64_t precomputed[] = {
		1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880,
		3628800, 39916800, 479001600, 6227020800, 87178291200,
		1307674368000, 20922789888000, 355687428096000,
		6402373705728000, 121645100408832000, 2432902008176640000
	};

	return precomputed[n];
}

constexpr int64_t permutation_index(const std::ranges::random_access_range auto& permutation)
{
	int64_t size = std::ranges::distance(permutation);
	int64_t index = 0;
	uint64_t bitboard = 0;
	for (int64_t i = 0; i < size; ++i)
	{
		uint64_t mask = 1ULL << permutation[i];

		// Number of remaining elements smaller than the current element
		// (total number of elements smaller than the current element) - (number of visited elements smaller than the current element)
		int64_t smaller = permutation[i] - std::popcount(bitboard & (mask - 1));

		// Total number of elements bigger than the current element
		int64_t bigger = size - i - 1;

		index += smaller * factorial(bigger);
		bitboard |= mask;
	}
	return index;
}

void nth_permutation(int64_t index, auto out_it, int64_t size)
{
	uint64_t unused = 0xFFFFFFFFFFFFFFULL;
	for (int64_t i = size - 1; i >= 0; --i)
	{
		int64_t f = factorial(i);
		uint64_t mask = _pdep_u64(1ULL << (index / f), unused);
		index %= f;
		*out_it++ = std::countr_zero(mask);
		unused ^= mask;
	}
}

void nth_permutation(int64_t index, std::ranges::range auto& out)
{
	nth_permutation(index, std::ranges::begin(out), std::ranges::size(out));
}

template <std::ranges::random_access_range R>
bool is_even_permutation(const R& permutation)
{
	int64_t size = std::ranges::distance(permutation);
	int64_t count = 0;
	for (int64_t i = 0; i < size; i++)
		for (int64_t j = i + 1; j < size; j++)
			if (permutation[i] > permutation[j])
				count++;
	return count % 2 == 0;
}

bool is_even_permutation(uint64_t lexicographical_index);

template <std::ranges::random_access_range R>
bool is_odd_permutation(const R& permutation)
{
	return not is_even_permutation(permutation);
}

bool is_odd_permutation(uint64_t lexicographical_index);