#pragma once
#include "std2.h"
#include "tables.h"
#include "cube.h"
#include <stdexcept>
#include <vector>

class TwoPhaseSolver
{
	const DistanceTable& phase_1;
	const DistanceTable& phase_2;
	const DistanceTable& corners;
	HashTable<Cube, uint8_t> tt{ 1'000'000, Cube::impossible(), 0 };
	std::vector<Twist> twists;
	int max_solution_length;

	void search(const Cube&, int p1_depth, Twist last = Twist::None);
public:
	TwoPhaseSolver();

	std::vector<Twist> solve(const Cube&, uint8_t max_solution_length);
};
