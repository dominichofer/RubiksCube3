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
	std::vector<Twist> solution(Cube cube, F1 index) const
	{
		std::vector<Twist> ret;
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

	auto hash() const { return std::hash<std::string_view>{}(std::string_view{ reinterpret_cast<const char*>(table.data()), table.size() }); }
};

class DirectionsTable
{
	std::vector<uint64_t> table;
public:
	DirectionsTable(std::size_t index_space) : table(index_space)
	{
	}

	template <typename Cube, typename F1, typename F2>
	void fill(const Cube& origin, Twists twists, F1 index, F2 from_index)
	{
		DistanceTable dt(twists, table.size());
		dt.fill(origin, index, from_index);

		const int64_t size = static_cast<int64_t>(table.size());
		#pragma omp parallel for schedule(static, 128)
		for (int64_t i = 0; i < size; i++)
		{
			Cube cube = from_index(i);
			uint8_t dst = dt.distance(i);
			uint64_t less_dst = 0;
			uint64_t more_dst = 0;
			for (Twist t : twists)
			{
				Cube n = cube.twisted(t);
				uint8_t n_dst = dt.distance(index(n));
				if (n_dst < dst)
					less_dst |= (1ULL << std::to_underlying(t));
				if (n_dst > dst)
					more_dst |= (1ULL << std::to_underlying(t));
			}
			table[i] = (less_dst << 32) | (more_dst << 8) | dst;
		}
	}

	uint8_t distance(std::size_t index) const { return static_cast<uint8_t>(table[index]); }
	Twists less_distance(std::size_t index) const { return Twists{ static_cast<uint32_t>(table[index] >> 32) }; }
	Twists more_distance(std::size_t index) const { return Twists{ static_cast<uint32_t>((table[index] >> 8) & 0xFF'FF'FFULL) }; }

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

	auto hash() const { return std::hash<std::string_view>{}(std::string_view{ reinterpret_cast<const char*>(table.data()), table.size() }); }
};

DistanceTable Corners_distance_table(std::string filename = "");
DistanceTable Subset_distance_table(std::string filename = "");
DirectionsTable Direction_to_subset_table(std::string filename = "");
