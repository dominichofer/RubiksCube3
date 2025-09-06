#include "two_phase.h"
#include <iostream>

void TwoPhaseSolver::search(const Cube& cube, int p1_depth, Twist last)
{
	const bool in_subset = cube.in_subset();
	if (p1_depth == 0 and in_subset)
	{
		phase2_probes++;
		if (phase_2.distance(cube.coset_index()) + twists.size() <= max_solution_length)
		{
			auto phase2_solution = phase_2.solution(cube, [](const Cube& c) { return c.coset_index(); });
			twists.insert(twists.end(), phase2_solution.begin(), phase2_solution.end());
			throw twists;
		}
	}
	if (p1_depth == 0 or in_subset)
		return;

	const uint64_t coset_number = cube.coset_number();
	const uint8_t p1_dst = phase_1.distance(coset_number);
	if (p1_dst > p1_depth)
		return;

	//if (p1_dst < p1_depth and p1_depth + p1_dst < 5)
	//	return;

	//if (corners.distance(cube.corners_index()) > max_solution_length - twists.size())
	//	return;

	Twists current_twists = (p1_depth == 1 ? ~Twists::H0() : Twists::all());
	current_twists.erase_face(last);
	if (p1_dst == p1_depth)
		current_twists.keep(phase_1.less_distance(coset_number));
	if (p1_dst + 1 == p1_depth)
		current_twists.remove(phase_1.more_distance(coset_number));
	for (Twist t : current_twists)
	{
		twists.push_back(t);
		search(cube.twisted(t), p1_depth - 1, t);
		twists.pop_back();
	}
}

TwoPhaseSolver::TwoPhaseSolver(
	DirectionsTable phase_1,
	DistanceTable phase_2,
	DistanceTable corners
)
	: phase_1(std::move(phase_1))
	, phase_2(std::move(phase_2))
	, corners(std::move(corners))
{
}

std::vector<Twist> TwoPhaseSolver::solve(const Cube& cube, uint8_t max_solution_length)
{
	twists.clear();
	twists.reserve(max_solution_length);
	this->max_solution_length = max_solution_length;
	phase2_probes = 0;
	try
	{
		for (int d = phase_1.distance(cube.coset_number()); d <= max_solution_length; d++)
			search(cube, d);
	}
	catch (const std::vector<Twist>& solution)
	{
		return solution;
	}
	throw std::runtime_error("No solution found");
}
