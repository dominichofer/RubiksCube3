#include "tables.h"
#include "corners.h"
#include "cube.h"
#include <chrono>
#include <iostream>

template <typename Cube, typename F1, typename F2>
static const DistanceTable& get_distance_table(
	const char* filename,
	Twists twists,
	F1 index,
	F2 from_index,
	std::size_t index_space,
	const Cube& origin)
{
	static DistanceTable table{ std::move(twists), index_space };
	static bool initialized = false;
	if (!initialized)
	{
		try
		{
			table.read(filename);
		}
		catch (...)
		{
			std::cout << "Creating distance table '" << filename << "' ...";
			auto start = std::chrono::high_resolution_clock::now();
			table.fill(origin, index, from_index);
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
			std::cout << " done. ("
				<< duration.count() << " s)" << std::endl;
			table.write(filename);
		}
		initialized = true;
	}
	return table;
}

const DistanceTable& Corners_distance_table()
{
	return get_distance_table(
		"..\\corners.dst",
		Twists::all(),
		[](const Corners& c) { return c.index(); },
		[](uint64_t i) { return Corners::from_index(i); },
		Corners::index_size,
		Corners{}
	);
}

const DistanceTable& H0_solution_distance_table()
{
	return get_distance_table(
		"..\\subset.dst",
		Twists::H0(),
		[](const Cube& c) { return c.coset_index(); },
		[](uint64_t i) { return Cube::from_subset(i); },
		Cube::set_size,
		Cube{}
	);
}

const DistanceTable& H0_subset_distance_table()
{
	return get_distance_table(
		"..\\coset.dst",
		Twists::all(),
		[](const Cube& c) { return c.coset_number(); },
		[](uint64_t i) { return Cube::from_coset(i, 0); },
		Cube::cosets,
		Cube{}
	);
}
