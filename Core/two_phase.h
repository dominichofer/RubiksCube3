#pragma once
#include "std2.h"
#include "tables.h"
#include "cube.h"
#include <stdexcept>
#include <vector>

class TwoPhaseSolver
{
	DirectionsTable phase_1;
	DistanceTable phase_2;
	DistanceTable corners;
	std::vector<Twist> twists;
	int max_solution_length = 20;
	int phase2_probes = 0;

	void search(const Cube&, int p1_depth, Twist last = Twist::None);
public:
	TwoPhaseSolver(
		DirectionsTable phase_1,
		DistanceTable phase_2,
		DistanceTable corners
	);

	std::vector<Twist> solve(const Cube&, uint8_t max_solution_length);
};
