#include "pch.h"

static std::string twisted_string(Twist t)
{
	return to_string(Edges{}.twisted(t));
}

TEST(Edges, L1) { EXPECT_EQ(twisted_string(Twist::L1), "0 1 2 3 11 5 6 8 4 9 10 7 | 0 0 0 0 1 0 0 1 1 0 0 1"); }
TEST(Edges, R1) { EXPECT_EQ(twisted_string(Twist::R1), "0 1 2 3 4 9 10 7 8 6 5 11 | 0 0 0 0 0 1 1 0 0 1 1 0"); }
TEST(Edges, U1) { EXPECT_EQ(twisted_string(Twist::U1), "5 4 2 3 0 1 6 7 8 9 10 11 | 0 0 0 0 0 0 0 0 0 0 0 0"); }
TEST(Edges, D1) { EXPECT_EQ(twisted_string(Twist::D1), "0 1 6 7 4 5 3 2 8 9 10 11 | 0 0 0 0 0 0 0 0 0 0 0 0"); }
TEST(Edges, F1) { EXPECT_EQ(twisted_string(Twist::F1), "8 1 2 9 4 5 6 7 3 0 10 11 | 0 0 0 0 0 0 0 0 0 0 0 0"); }
TEST(Edges, B1) { EXPECT_EQ(twisted_string(Twist::B1), "0 10 11 3 4 5 6 7 8 9 2 1 | 0 0 0 0 0 0 0 0 0 0 0 0"); }

TEST(Edges, composed_twists)
{
	const auto c = Edges{};
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

TEST(Edges, inverse_twists)
{
	for (Twist t : Twists::all())
		EXPECT_TRUE(Edges{}.twisted({ t, inversed(t) }).is_solved());
}

TEST(Edges, twists_cycle)
{
	for (Twist t : Twists::all())
		EXPECT_TRUE(Edges{}.twisted({ t, t, t, t }).is_solved());
}

static void expect_twists_commutate(Twist a, Twist b)
{
	EXPECT_EQ(Edges{}.twisted({ a, b }), Edges{}.twisted({ b, a }));
}

TEST(Edges, twist_commutation)
{
	expect_twists_commutate(Twist::L1, Twist::R1);
	expect_twists_commutate(Twist::U1, Twist::D1);
	expect_twists_commutate(Twist::F1, Twist::B1);
}

TEST(Edges, is_solved)
{
	EXPECT_TRUE(Edges{}.is_solved());
	EXPECT_FALSE(Edges{}.twisted(Twist::L1).is_solved());
}

TEST(Edges, index)
{
	RandomCubeGenerator<Edges> gen{ 928322 };
	for (int i = 0; i < 1'000'000; i++)
	{
		Edges e1 = gen();

		auto slice_prm = e1.slice_prm_index();
		ASSERT_GE(slice_prm, 0);
		ASSERT_LT(slice_prm, Edges::slice_prm_size);

		auto non_slice_prm = e1.non_slice_prm_index();
		ASSERT_GE(non_slice_prm, 0);
		ASSERT_LT(non_slice_prm, Edges::non_slice_prm_size);

		auto slice_loc = e1.slice_loc_index();
		ASSERT_GE(slice_loc, 0);
		ASSERT_LT(slice_loc, Edges::slice_loc_size);

		auto ori = e1.ori_index();
		ASSERT_GE(ori, 0);
		ASSERT_LT(ori, Edges::ori_size);

		auto e2 = Edges::from_index(slice_prm, non_slice_prm, slice_loc, ori);
		EXPECT_EQ(e1, e2);
	}
}