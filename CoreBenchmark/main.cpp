#include "benchmark/benchmark.h"
#include "Core/core.h"
#include <array>
#include <iostream>
#include <random>
#include <vector>

namespace math_benchmarks
{
	static std::vector<int> random_combination(int n, int k)
	{
		std::vector<int> c;
		for (int i = 0; i < k; ++i)
			c.push_back(rand() % n);
		std::ranges::sort(c);
		return c;
	}

	static std::vector<int> random_permutation(int size)
	{
		std::vector<int> p(size);
		std::ranges::iota(p, 0);
		std::ranges::shuffle(p, std::mt19937{ std::random_device{}() });
		return p;
	}

	void combination_index(benchmark::State& state)
	{
		std::vector<int> comb = random_combination(12, 4);
		for (auto _ : state)
			benchmark::DoNotOptimize(::combination_index(12, comb));
	}
	BENCHMARK(combination_index);

	void nth_combination(benchmark::State& state)
	{
		std::array<uint8_t, 4> arr;
		int index = rand() % binomial(12, 4);
		for (auto _ : state)
		{
			::nth_combination(12, 4, index, arr);
			benchmark::DoNotOptimize(arr);
		}
	}
	BENCHMARK(nth_combination);

	void permutation_index(benchmark::State& state)
	{
		int n = state.range(0);
		std::vector<int> perm = random_permutation(n);
		for (auto _ : state)
			benchmark::DoNotOptimize(::permutation_index(perm));
	}
	BENCHMARK(permutation_index)->DenseRange(4, 12, 4);

	void nth_permutation(benchmark::State& state)
	{
		int n = state.range(0);
		uint64_t index = rand() % factorial(n);
		std::vector<int> p(n);
		for (auto _ : state)
		{
			::nth_permutation(index, p);
			benchmark::DoNotOptimize(p);
		}
	}
	BENCHMARK(nth_permutation)->DenseRange(4, 12, 4);

	void is_even_permutation_range(benchmark::State& state)
	{
		int n = state.range(0);
		std::vector<int> perm = random_permutation(n);
		for (auto _ : state)
			benchmark::DoNotOptimize(is_even_permutation(perm));
	}
	BENCHMARK(is_even_permutation_range)->DenseRange(4, 12, 4);

	void is_even_permutation_index(benchmark::State& state)
	{
		int n = state.range(0);
		auto index = rand() % factorial(n);
		for (auto _ : state)
			benchmark::DoNotOptimize(is_even_permutation(index));
	}
	BENCHMARK(is_even_permutation_index)->DenseRange(4, 12, 4);
}

template <typename Cube>
uint64_t perft(const Cube& cube, int depth)
{
	if (depth == 0)
		return 1;
	uint64_t sum = 0;
	for (Twist t : all_twists)
		sum += perft(cube.twisted(t), depth - 1);
	return sum;
}

template <typename Cube>
void perft()
{
	auto start = std::chrono::high_resolution_clock::now();
	uint64_t counter = perft(Cube{}, /*depth*/ 7);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	std::cout << typeid(Cube).name() << " " << counter / 1000 / duration << " MN/s" << std::endl;
}

#define BENCH(cls, func) \
void cls##_##func(benchmark::State& state) \
{ \
	auto cube = RandomCube<cls>(); \
	for (auto _ : state) \
		benchmark::DoNotOptimize(cube.func()); \
} \
BENCHMARK(cls##_##func);


BENCH(Corners, is_solved)
BENCH(Corners, prm_index)
BENCH(Corners, ori_index)
BENCH(Corners, index)

void Corners_from_index_prm_ori(benchmark::State& state)
{
	auto cube = RandomCube<Corners>(423749);
	auto prm = cube.prm_index();
	auto ori = cube.ori_index();
	for (auto _ : state)
		benchmark::DoNotOptimize(Corners::from_index(prm, ori));
}
BENCHMARK(Corners_from_index_prm_ori);

void Corners_from_index(benchmark::State& state)
{
	auto cube = RandomCube<Corners>(423749);
	auto index = cube.index();
	for (auto _ : state)
		benchmark::DoNotOptimize(Corners::from_index(index));
}
BENCHMARK(Corners_from_index);


BENCH(Edges, is_solved)
BENCH(Edges, slice_prm_index)
BENCH(Edges, non_slice_prm_index)
BENCH(Edges, slice_loc_index)
BENCH(Edges, ori_index)

void Edges_from_index(benchmark::State& state)
{
	auto cube = RandomCube<Edges>(423749);
	auto a = cube.slice_prm_index();
	auto b = cube.non_slice_prm_index();
	auto c = cube.slice_loc_index();
	auto d = cube.ori_index();
	for (auto _ : state)
		benchmark::DoNotOptimize(Edges::from_index(a, b, c, d));
}
BENCHMARK(Edges_from_index);


BENCH(Cube, in_subset)
BENCH(Cube, is_solved)
BENCH(Cube, coset_index)
BENCH(Cube, coset_number)

void Cube_from_subset(benchmark::State& state)
{
	auto cube = RandomCube<Cube>(423749);
	auto index = cube.coset_index();
	for (auto _ : state)
		benchmark::DoNotOptimize(Cube::from_subset(index));
}
BENCHMARK(Cube_from_subset);

void Cube_from_coset_number(benchmark::State& state)
{
	auto cube = RandomCube<Cube>(423749);
	auto index = cube.coset_index();
	auto number = cube.coset_number();
	for (auto _ : state)
		benchmark::DoNotOptimize(Cube::from_coset(number, index));
}
BENCHMARK(Cube_from_coset_number);

int main(int argc, char** argv)
{
	perft<Corners>();
	perft<Edges>();
	perft<Cube>();
	std::cout << std::endl;

	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
	::benchmark::RunSpecifiedBenchmarks();
	::benchmark::Shutdown();
	return 0;
}
