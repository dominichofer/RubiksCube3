#include "two_phase.h"

void TwoPhaseSolver::search(const Cube& cube, int p1_depth, Twist last)
{
	bool in_subset = cube.in_subset();
	if (p1_depth == 0 and in_subset)
	{
		if (phase_2.distance(cube.coset_index()) + twists.size() <= max_solution_length)
		{
			auto phase2_solution = phase_2.solution(cube, [](const Cube& c) { return c.coset_index(); });
			twists.insert(twists.end(), phase2_solution.begin(), phase2_solution.end());
			throw twists;
		}
	}
	if (p1_depth == 0 or in_subset)
		return;

	if (phase_1.distance(cube.coset_number()) > p1_depth)
		return;

	if (corners.distance(cube.corners_index()) > max_solution_length - twists.size())
		return;

	if (tt.retrieve(cube).value_or(0) >= p1_depth)
		return;

	Twists current_twists = (p1_depth == 1 ? ~Twists::H0() : Twists::all());
	current_twists.erase_face(last);
	for (Twist t : current_twists)
	{
		twists.push_back(t);
		search(cube.twisted(t), p1_depth - 1, t);
		twists.pop_back();
	}

	tt.insert(cube, p1_depth);
}

TwoPhaseSolver::TwoPhaseSolver()
	: phase_1(H0_subset_distance_table())
	, phase_2(H0_solution_distance_table())
	, corners(Corners_distance_table())
{
}

std::vector<Twist> TwoPhaseSolver::solve(const Cube& cube, uint8_t max_solution_length)
{
	twists.clear();
	twists.reserve(max_solution_length);
	this->max_solution_length = max_solution_length;
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
