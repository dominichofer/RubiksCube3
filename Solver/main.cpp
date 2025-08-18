#include "Core/core.h"
#include <chrono>
#include <iostream>

int main()
{
	TwoPhaseSolver solver;
	RandomCubeGenerator<Cube> rnd{ /*seed*/ 323470 };
	auto start = std::chrono::high_resolution_clock::now();
	int N = 100;
	for (int i = 0; i < N; i++)
	{
		auto cube = rnd(100);
		auto start_ = std::chrono::high_resolution_clock::now();
		auto sol = solver.solve(cube, 20);
		auto stop_ = std::chrono::high_resolution_clock::now();
		std::cout << "Solution time: " << to_string(stop_ - start_) << std::endl;
		if (not cube.twisted(sol).is_solved())
			std::cerr << "Solution not found " << sol << std::endl;
		if (sol.size() > 20)
			std::cerr << "Solution too long: " << sol.size() << std::endl;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Avg solution time: " << to_string((stop - start) / N) << std::endl;
}
