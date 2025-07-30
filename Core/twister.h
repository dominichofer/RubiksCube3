#pragma once
#include "twist.h"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

template <typename Cube, typename index_t>
class AllTwister
{
	std::vector<index_t> table;
public:
	AllTwister(
		std::function<index_t(Cube)> index,
		std::function<Cube(uint64_t)> from_index,
		std::size_t index_space)
		: table(index_space * all_twists.size())
	{
		const int64_t size = static_cast<int64_t>(index_space);
		#pragma omp parallel for
		for (int64_t i = 0; i < size; i++)
			for (Twist t : all_twists)
				table[i * all_twists.size() + std::to_underlying(t)] = index(from_index(i).twisted(t));
	}

	index_t operator()(uint64_t i, Twist t) const
	{
		return table[i * all_twists.size() + std::to_underlying(t)];
	}
};
