#include "math.h"

bool is_even_permutation(uint64_t lexicographical_index)
{
	// Convert the index to its factoradic representation and sum the digits.
	uint64_t sum = 0;
	for (uint64_t i = 2; lexicographical_index > 0; i++)
	{
		sum += lexicographical_index % i;
		lexicographical_index /= i;
	}
	return sum % 2 == 0;
}

bool is_odd_permutation(uint64_t lexicographical_index)
{
	return not is_even_permutation(lexicographical_index);
}