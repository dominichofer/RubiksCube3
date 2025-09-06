#include "tables.h"
#include "corners.h"
#include "cube.h"
#include <chrono>
#include <iostream>

DistanceTable Corners_distance_table(std::string filename)
{
	auto start = std::chrono::high_resolution_clock::now();
	DistanceTable table{ Twists::all(), Corners::index_size };
	if (filename != "" and std::ifstream(filename).good())
	{
		std::cout << "Loading corners distance table " << filename << " ... ";
		table.read(filename);
	}
	else
	{
		std::cout << "Creating corners distance table " << filename << " ... ";
		table.fill(
			Corners{},
			[](const Corners& c) { return c.index(); },
			[](uint64_t i) { return Corners::from_index(i); }
		);
		if (filename != "")
			table.write(filename);
	}
	std::cout << "done. (" << to_string(std::chrono::high_resolution_clock::now() - start) << ")" << std::endl;
	std::cout << "Hash: " << table.hash() << std::endl;
	return table;
}

DistanceTable Subset_distance_table(std::string filename)
{
	auto start = std::chrono::high_resolution_clock::now();
	DistanceTable table{ Twists::H0(), Cube::set_size };
	if (filename != "" and std::ifstream(filename).good())
	{
		std::cout << "Loading subset distance table " << filename << " ... ";
		table.read(filename);
	}
	else
	{
		std::cout << "Creating subset distance table " << filename << " ... ";
		table.fill(
			Cube{},
			[](const Cube& c) { return c.coset_index(); },
			[](uint64_t i) { return Cube::from_subset(i); }
		);
		if (filename != "")
			table.write(filename);
	}
	std::cout << "done. (" << to_string(std::chrono::high_resolution_clock::now() - start) << ")" << std::endl;
	std::cout << "Hash: " << table.hash() << std::endl;
	return table;
}

DirectionsTable Direction_to_subset_table(std::string filename)
{
	auto start = std::chrono::high_resolution_clock::now();
	DirectionsTable table{ Cube::cosets };
	if (filename != "" and std::ifstream(filename).good())
	{
		std::cout << "Loading coset direction table " << filename << " ... ";
		table.read(filename);
	}
	else
	{
		std::cout << "Creating coset direction table " << filename << " ... ";
		table.fill(
			Cube{},
			Twists::all(),
			[](const Cube& c) { return c.coset_number(); },
			[](uint64_t i) { return Cube::from_coset(i, 0); }
		);
		if (filename != "")
			table.write(filename);
	}
	std::cout << "done. (" << to_string(std::chrono::high_resolution_clock::now() - start) << ")" << std::endl;
	std::cout << "Hash: " << table.hash() << std::endl;
	return table;
}
