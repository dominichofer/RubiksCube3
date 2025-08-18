#pragma once
#include "twist.h"
#include <random>

class RandomTwistGenerator
{
	std::mt19937_64 rng;
	std::uniform_int_distribution<std::size_t> dist;
	Twists twists;
public:
	RandomTwistGenerator(Twists twists, unsigned int seed = std::random_device{}())
		: rng(seed)
		, twists(std::move(twists))
		, dist(0, twists.size() - 1)
	{
	}
	RandomTwistGenerator(unsigned int seed = std::random_device{}())
		: RandomTwistGenerator(Twists::all(), seed)
	{
	}

	Twist operator()()
	{
		return twists[dist(rng)];
	}

	std::vector<Twist> operator()(int count)
	{
		std::vector<Twist> result;
		result.reserve(count);
		for (int i = 0; i < count; i++)
			result.push_back(this->operator()());
		return result;
	}
};

template <typename Cube>
class RandomCubeGenerator
{
	RandomTwistGenerator rnd_twists;
	Cube origin;
public:
	RandomCubeGenerator(Cube origin, Twists twists, unsigned int seed = std::random_device{}())
		: rnd_twists(twists, seed)
		, origin(origin)
	{
	}
	RandomCubeGenerator(unsigned int seed = std::random_device{}())
		: RandomCubeGenerator(Cube{}, Twists::all(), seed)
	{
	}

	Cube operator()(int twist_count = 100)
	{
		Cube cube = origin;
		for (int i = 0; i < twist_count; i++)
			cube = cube.twisted(rnd_twists());
		return cube;
	}
};

template <typename Cube>
Cube RandomCube(Cube origin, Twists twists, unsigned int seed = std::random_device{}())
{
	return RandomCubeGenerator{ origin, twists, seed }();
}

template <typename Cube>
Cube RandomCube(unsigned int seed = std::random_device{}())
{
	return RandomCubeGenerator<Cube>{ seed }();
}

template <typename Cube>
std::vector<Cube> RandomCubes(int count, unsigned int seed = std::random_device{}())
{
	std::vector<Cube> cubes;
	cubes.reserve(count);
	RandomCubeGenerator<Cube> rnd{ seed };
	for (int i = 0; i < count; i++)
		cubes.push_back(rnd());
	return cubes;
}
