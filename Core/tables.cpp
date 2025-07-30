#include "tables.h"
#include "corners.h"
#include "cube.h"

template <typename Cube, typename F1, typename F2>
static const DistanceTable& get_distance_table(
	const char* filename,
	Twists twists,
	const Cube& origin,
	F1 index,
	F2 from_index,
	std::size_t index_space)
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
			table.fill(origin, index, from_index);
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
		all_twists,
		Corners{},
		&Corners::index,
		&Corners::from_index,
		Corners::index_size
	);
}

const DistanceTable& H0_solution_distance_table()
{
	return get_distance_table(
		"..\\subset.dst",
		H0::twists,
		Cube{},
		&Cube::coset_index,
		&Cube::from_subset,
		Cube::set_size
	);
}

const DistanceTable& H0_subset_distance_table()
{
	return get_distance_table(
		"..\\coset.dst",
		all_twists,
		Cube{},
		&Cube::coset_number,
		[](uint64_t i) { return Cube::from_coset(i, 0); },
		Cube::cosets
	);
}
