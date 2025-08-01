#pragma once
#include "twist.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <vector>

class DistanceTable
{
	Twists twists;
	std::vector<uint8_t> table;
public:
	DistanceTable() = default;
	DistanceTable(Twists twists, std::size_t index_space)
		: twists(std::move(twists)), table(index_space)
	{
	}

	template <typename Cube, typename F1, typename F2>
	void fill(const Cube& origin, F1 index, F2 from_index)
	{
		const int64_t size = static_cast<int64_t>(table.size());
		constexpr uint8_t sentinel = std::numeric_limits<uint8_t>::max();
		std::ranges::fill(table, sentinel);
		table[index(origin)] = 0;
		for (uint8_t d = 0; d < sentinel - 1; d++)
		{
			bool changed = false;
			#pragma omp parallel for reduction(||: changed) schedule(static, 128)
			for (int64_t i = 0; i < size; i++)
				if (table[i] == d)
				{
					Cube cube = from_index(i);
					for (Twist t : twists)
					{
						Cube n = cube.twisted(t);
						uint8_t& n_d = table[index(n)];
						if (n_d == sentinel)
						{
							n_d = d + 1;
							changed = true;
						}
					}
				}
			if (not changed)
				break;
		}
	}

	uint8_t distance(std::size_t index) const { return table[index]; }

	void read(std::fstream& file)
	{
		file.read(reinterpret_cast<char*>(table.data()), table.size());
	}
	void read(const std::string& file)
	{
		std::fstream f(file, std::ios::binary | std::ios::in);
		if (!f.is_open())
			throw std::runtime_error("Failed to open file: " + file);
		read(f);
	}

	void write(std::fstream& file) const
	{
		file.write(reinterpret_cast<const char*>(table.data()), table.size());
	}
	void write(const std::string& file) const
	{
		std::fstream f(file, std::ios::binary | std::ios::out);
		if (!f.is_open())
			throw std::runtime_error("Failed to open file: " + file);
		write(f);
	}
	auto begin() const { return table.begin(); }
	auto end() const { return table.end(); }

	template <typename Cube, typename F1>
	Twists solution(Cube cube, F1 index) const
	{
		Twists ret;
		for (uint8_t d = distance(index(cube)); d > 0; d--)
			for (Twist t : twists)
			{
				Cube n = cube.twisted(t);
				if (distance(index(n)) == d - 1)
				{
					ret.push_back(t);
					cube = n;
					break;
				}
			}
		return ret;
	}
};

// Distance to the solved state of a Cube3x3.
const DistanceTable& Corners_distance_table();

// Distance to the solved state of a Cube3x3 in the H0 subset.
const DistanceTable& H0_solution_distance_table();

// Distance to the H0 subset.
const DistanceTable& H0_subset_distance_table();