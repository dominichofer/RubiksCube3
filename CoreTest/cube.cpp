#include "pch.h"

TEST(Cube, from_subset)
{
	auto ref_number = Cube{}.coset_number();

	RandomCubeGenerator<Cube> gen{ Cube{}, H0::twists, 928322 };
	for (int i = 0; i < 1'000'000; i++)
	{
		Cube c1 = gen();

		auto index = c1.coset_index();
		ASSERT_GE(index, 0);
		ASSERT_LT(index, Cube::set_size);

		auto number = c1.coset_number();
		ASSERT_EQ(number, ref_number);

		auto c2 = Cube::from_subset(index);
		ASSERT_EQ(c1, c2);
	}
}

TEST(Cube, from_coset)
{
	RandomCubeGenerator<Cube> gen{ 928322 };
	for (int i = 0; i < 1'000'000; i++)
	{
		Cube c1 = gen();

		auto index = c1.coset_index();
		ASSERT_GE(index, 0);
		ASSERT_LT(index, Cube::set_size);

		auto number = c1.coset_number();
		ASSERT_GE(number, 0);
		ASSERT_LT(number, Cube::cosets);

		auto c2 = Cube::from_coset(number, index);
		ASSERT_EQ(c1, c2);
	}
}
