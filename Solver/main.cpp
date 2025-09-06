#include "core.h"
#include <chrono>
#include <iostream>

//class CosetCover
//{
//	DirectionsTable phase_1;
//	const DistanceTable& phase_2;
//	const DistanceTable& corners;
//	std::vector<bool> coset;
//	std::vector<Twist> twists;
//	int max_solution_length;
//	uint64_t coset_number;
//public:
//
//	CosetCover(int max_solution_length = 20)
//		: phase_1(Twists::all(), Cube::cosets)
//		, phase_2(H0_solution_distance_table())
//		, corners(Corners_distance_table())
//		, coset(Cube::cosets, false)
//		, max_solution_length(max_solution_length)
//	{
//	}
//
//	void reset(uint64_t coset_number)
//	{
//		this->coset_number = coset_number;
//		std::ranges::fill(coset, false);
//	}
//
//	void cover_with(const std::vector<Twist>& twists)
//	{
//		auto start = std::chrono::high_resolution_clock::now();
//		auto twists_count = twists.size();
//		int64_t size = static_cast<int64_t>(coset.size());
//
//		#pragma omp parallel for
//		for (uint16_t e_non_slice_prm = 0; e_non_slice_prm < Edges::non_slice_prm_size; e_non_slice_prm++)
//		{
//			for (uint8_t e_slice_prm = 0; e_slice_prm < Edges::slice_prm_size; e_slice_prm++)
//			{
//				auto coset_cube = Cube{};
//				coset_cube.e_slice_prm = e_slice_prm;
//				coset_cube.e_non_slice_prm = e_non_slice_prm;
//				coset_cube = coset_cube.twisted(twists);
//
//				bool even_edge_prm = is_even_permutation(e_non_slice_prm) xor is_even_permutation(e_slice_prm); // TODO: Add e_slice_loc!
//				auto& lookup_table = even_edge_prm ? even_lookup_table : odd_lookup_table;
//
//				for (uint16_t x = 0; x < Corners::prm_size / 2; x++)
//				{
//					if (phase_2.distance(i) + twists_count > max_solution_length)
//						continue;
//
//					uint16_t c_prm = lookup_table[x];
//					coset_cube.c_prm = c_prm;
//					coset[coset_cube.coset_index()] = true;
//				}
//			}
//		}
//		uint8_t e_slice_prm; // 4! = 24
//		uint16_t e_non_slice_prm; // 8! = 40'320
//		for (int64_t i = 0; i < size; i++)
//		{
//			if (phase_2.distance(i) + twists_count > max_solution_length)
//				continue;
//			Cube subset_cube = Cube3x3::from_subset(i);
//			Cube coset_cube = subset_cube.twisted(twists);
//			auto index = coset_cube.coset_index();
//			coset[index] = true;
//		}
//		auto end = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//		uint64_t covered_count = std::ranges::count(coset, true);
//		std::cout << "Covering coset " << coset_number << " with "
//			<< (int)twists_count << " twists took " << duration << " ms, "
//			<< "covered count: " << covered_count << ", %: " << covered_count * 100.0 / size << std::endl;
//	}
//
//	void search(const Cube& cube, int p1_depth, Twist last = Twist::None)
//	{
//		bool in_subset = cube.in_subset();
//		if (p1_depth == 0 and in_subset)
//		{
//			cover_with(inversed(twists));
//			return;
//		}
//		if (p1_depth == 0 or in_subset)
//			return;
//
//		uint64_t coset_number = cube.coset_number();
//		uint8_t p1_dst = phase_1.distance(coset_number);
//		if (p1_dst > p1_depth)
//			return;
//
//		//if (corners.distance(cube.corners_index()) > max_solution_length - twists.size())
//		//	return;
//
//		if (corners.distance(cube.corners_index()) > max_solution_length - twists.size())
//			return;
//
//		Twists current_twists = (p1_depth == 1 ? ~Twists::H0() : Twists::all());
//		current_twists.erase_face(last);
//		if (p1_dst == p1_depth)
//			current_twists.filter(phase_1.to_solution(coset_number));
//		if (p1_dst + 1 == p1_depth)
//			current_twists.filter(phase_1.not_away(coset_number));
//		for (Twist t : current_twists)
//		{
//			twists.push_back(t);
//			search(cube.twisted(t), p1_depth - 1, t);
//			twists.pop_back();
//		}
//	}
//
//	void cover(int depth)
//	{
//		Cube cube = Cube::from_coset(coset_number, 0);
//		search(cube, depth);
//	}
//};

const DirectionsTable& distance_to_subset_table()
{
	auto filename = "..\\coset.dst";
	static DirectionsTable table{ Cube::cosets };
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
			table.fill(
				Cube{},
				Twists::all(),
				[](const Cube& c) { return c.coset_number(); },
				[](uint64_t i) { return Cube::from_coset(i, 0); }
			);
			auto stop = std::chrono::high_resolution_clock::now();
			std::cout << " done. (" << to_string(stop - start) << ")" << std::endl;
			table.write(filename);
		}
		initialized = true;
	}
	return table;
}

int main()
{
	// Read config file
	std::ifstream config_file("../config.txt");
	if (not config_file.is_open())
	{
		std::cerr << "Missing config file '../config.txt'" << std::endl;
		return 1;
	}
	std::string corners_file, coset_file, subset_file;
	std::getline(config_file, corners_file);
	std::getline(config_file, coset_file);
	std::getline(config_file, subset_file);

	DirectionsTable phase_1 = Direction_to_subset_table(coset_file);
	DistanceTable phase_2 = Subset_distance_table(subset_file);
	DistanceTable corners = Corners_distance_table(corners_file);
	

	//CosetCover set{ 20 };
	//for (int nr = 0; nr < Cube::cosets; nr++)
	//{
	//	set.reset(nr);
	//	for (int i = 0; i < 10; i++)
	//	{
	//		auto start = std::chrono::high_resolution_clock::now();
	//		set.cover(i);
	//		auto end = std::chrono::high_resolution_clock::now();
	//		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	//		std::cout << "Covering coset " << nr << " with depth " << i
	//			<< " took " << duration << " ms" << std::endl;
	//	}
	//}

	TwoPhaseSolver solver{ std::move(phase_1), std::move(phase_2), std::move(corners) };
	RandomCubeGenerator<Cube> rnd{ /*seed*/ 323470 };
	auto duration = std::chrono::nanoseconds::zero();
	int N = 1000;
	for (int i = 0; i < N; i++)
	{
		auto cube = rnd(100);
		auto start = std::chrono::high_resolution_clock::now();
		auto sol = solver.solve(cube, 20);
		auto stop = std::chrono::high_resolution_clock::now();
		duration += stop - start;
		std::cout << to_string(stop - start) << std::endl;
		if (not cube.twisted(sol).is_solved())
			std::cerr << "Solution invalid: " << sol << std::endl;
		if (sol.size() > 20)
			std::cerr << "Solution too long: " << sol.size() << std::endl;
	}
	std::cout << "Avg solution time: " << to_string(duration / N) << std::endl;
}
