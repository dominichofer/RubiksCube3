#include "pch.h"
#include <string>

static std::string twisted_string(Twist t)
{
	return to_string(Corners{}.twisted(t));
}

TEST(Corners, L1) { EXPECT_EQ(twisted_string(Twist::L1), "2 1 6 3 0 5 4 7 | 2 0 2 0 2 0 2 0"); }
TEST(Corners, R1) { EXPECT_EQ(twisted_string(Twist::R1), "0 5 2 1 4 7 6 3 | 0 2 0 2 0 2 0 2"); }
TEST(Corners, U1) { EXPECT_EQ(twisted_string(Twist::U1), "1 3 0 2 4 5 6 7 | 0 0 0 0 0 0 0 0"); }
TEST(Corners, D1) { EXPECT_EQ(twisted_string(Twist::D1), "0 1 2 3 6 4 7 5 | 0 0 0 0 0 0 0 0"); }
TEST(Corners, F1) { EXPECT_EQ(twisted_string(Twist::F1), "4 0 2 3 5 1 6 7 | 1 1 0 0 1 1 0 0"); }
TEST(Corners, B1) { EXPECT_EQ(twisted_string(Twist::B1), "0 1 3 7 4 5 2 6 | 0 0 1 1 0 0 1 1"); }

TEST(Corners, composed_twists)
{
	const auto c = Corners{};
	EXPECT_EQ(c.twisted(Twist::L2), c.twisted({ Twist::L1, Twist::L1 }));
	EXPECT_EQ(c.twisted(Twist::L3), c.twisted({ Twist::L1, Twist::L1, Twist::L1 }));
	EXPECT_EQ(c.twisted(Twist::R2), c.twisted({ Twist::R1, Twist::R1 }));
	EXPECT_EQ(c.twisted(Twist::R3), c.twisted({ Twist::R1, Twist::R1, Twist::R1 }));
	EXPECT_EQ(c.twisted(Twist::U2), c.twisted({ Twist::U1, Twist::U1 }));
	EXPECT_EQ(c.twisted(Twist::U3), c.twisted({ Twist::U1, Twist::U1, Twist::U1 }));
	EXPECT_EQ(c.twisted(Twist::D2), c.twisted({ Twist::D1, Twist::D1 }));
	EXPECT_EQ(c.twisted(Twist::D3), c.twisted({ Twist::D1, Twist::D1, Twist::D1 }));
	EXPECT_EQ(c.twisted(Twist::F2), c.twisted({ Twist::F1, Twist::F1 }));
	EXPECT_EQ(c.twisted(Twist::F3), c.twisted({ Twist::F1, Twist::F1, Twist::F1 }));
	EXPECT_EQ(c.twisted(Twist::B2), c.twisted({ Twist::B1, Twist::B1 }));
	EXPECT_EQ(c.twisted(Twist::B3), c.twisted({ Twist::B1, Twist::B1, Twist::B1 }));
}

TEST(Corners, inverse_twists)
{
	for (Twist t : all_twists)
		EXPECT_TRUE(Corners{}.twisted({ t, inversed(t) }).is_solved());
}

TEST(Corners, twists_cycle)
{
	for (Twist t : all_twists)
		EXPECT_TRUE(Corners{}.twisted({ t, t, t, t }).is_solved());
}

static void expect_twists_commutate(Twist a, Twist b)
{
	EXPECT_EQ(Corners{}.twisted({ a, b }), Corners{}.twisted({ b, a }));
}

TEST(Corners, twist_commutation)
{
	expect_twists_commutate(Twist::L1, Twist::R1);
	expect_twists_commutate(Twist::U1, Twist::D1);
	expect_twists_commutate(Twist::F1, Twist::B1);
}

TEST(Corners, is_solved)
{
	EXPECT_TRUE(Corners{}.is_solved());
	EXPECT_FALSE(Corners{}.twisted(Twist::L1).is_solved());
}

TEST(Corners, index_1)
{
	#pragma omp parallel for
	for (int prm = 0; prm < Corners::prm_size; prm++)
		for (int ori = 0; ori < Corners::ori_size; ori++)
		{
			Corners c1 = Corners::from_index(prm, ori);

			auto prm2 = c1.prm_index();
			EXPECT_GE(prm2, 0);
			EXPECT_LT(prm2, Corners::prm_size);
			EXPECT_EQ(prm, prm2);

			auto ori2 = c1.ori_index();
			EXPECT_GE(ori2, 0);
			EXPECT_LT(ori2, Corners::ori_size);
			EXPECT_EQ(ori, ori2);
		}
}

TEST(Corners, index_2)
{
	#pragma omp parallel for
	for (int index = 0; index < Corners::index_size; index++)
	{
		Corners c1 = Corners::from_index(index);
		auto index2 = c1.index();
		EXPECT_GE(index2, 0);
		EXPECT_LT(index2, Corners::index_size);
		EXPECT_EQ(index, index2);
	}
}

TEST(Corners, to_string)
{
	EXPECT_EQ(to_string(Corners{}), "0 1 2 3 4 5 6 7 | 0 0 0 0 0 0 0 0");
}
