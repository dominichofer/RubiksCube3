#include "pch.h"

TEST(DistanceTable, Corners)
{
	DistanceTable table{
		Twists::all(),
		Corners::index_size
	};
	table.fill(Corners{}, [](const Corners& c) { return c.index(); }, [](uint64_t i) { return Corners::from_index(i); });

	// According to https://oeis.org/A080629
	EXPECT_EQ(std::ranges::count(table, 0), 1);
	EXPECT_EQ(std::ranges::count(table, 1), 18);
	EXPECT_EQ(std::ranges::count(table, 2), 243);
	EXPECT_EQ(std::ranges::count(table, 3), 2'874);
	EXPECT_EQ(std::ranges::count(table, 4), 28'000);
	EXPECT_EQ(std::ranges::count(table, 5), 205'416);
	EXPECT_EQ(std::ranges::count(table, 6), 1'168'516);
	EXPECT_EQ(std::ranges::count(table, 7), 5'402'628);
	EXPECT_EQ(std::ranges::count(table, 8), 20'776'176);
	EXPECT_EQ(std::ranges::count(table, 9), 45'391'616);
	EXPECT_EQ(std::ranges::count(table, 10), 15'139'616);
	EXPECT_EQ(std::ranges::count(table, 11), 64'736);
	EXPECT_EQ(std::ranges::count(table, 12), 0);

	RandomCubeGenerator<Corners> rnd{ 53412 };
	for (int i = 0; i < 100'000; i++)
	{
		Corners c = rnd();
		auto sol = table.solution(c, [](const Corners& c) { return c.index(); });
		EXPECT_TRUE(c.twisted(sol).is_solved());
		EXPECT_EQ(table.distance(c.index()), sol.size());
	}
}