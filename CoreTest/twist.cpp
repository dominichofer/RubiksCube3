#include "pch.h"

TEST(Twist, string)
{
	for (Twist t : Twists::all())
		EXPECT_EQ(t, twist_from_string(to_string(t)));
}

// Tests that the inverse of the inverse of a twist is the twist itself.
TEST(Twist, inversed)
{
	for (Twist t : Twists::all())
		EXPECT_EQ(t, inversed(inversed(t)));
}

TEST(Twists, inversed)
{
	Twist t0 = Twist::L1;
	Twist t1 = Twist::R2;
	Twist t2 = Twist::U3;
	std::vector<Twist> twists = { t0, t1, t2 };
	std::vector<Twist> ref = { inversed(t2), inversed(t1), inversed(t0) };
	EXPECT_EQ(inversed(twists), ref);
}

// TEST(Twist, is_same_face)
// {
// 	EXPECT_TRUE(same_face(Twist::L1, Twist::L2));
// 	EXPECT_FALSE(same_face(Twist::L1, Twist::R1));
// }
