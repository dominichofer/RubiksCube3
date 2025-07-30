#include "pch.h"

TEST(AllTwister, corners)
{
	const AllTwister<Corners, uint16_t> c_ori_table(
		&Corners::ori_index,
		[](uint16_t i) { return Corners::from_index(0, i); },
		Corners::ori_size
	);
	const AllTwister<Corners, uint16_t> c_prm_table(
		&Corners::prm_index,
		[](uint16_t i) { return Corners::from_index(i, 0); },
		Corners::prm_size
	);

	RandomTwistGenerator rnd_twists{ 928322 };
	auto twists = rnd_twists(1'000'000);

	auto c = Corners{};
	auto prm = c.prm_index();
	auto ori = c.ori_index();
	for (Twist t : twists)
	{
		c = c.twisted(t);
		prm = c_prm_table(prm, t);
		ori = c_ori_table(ori, t);
		EXPECT_EQ(c.prm_index(), prm);
		EXPECT_EQ(c.ori_index(), ori);
	}
}

TEST(AllTwister, edges)
{
	const AllTwister<Edges, uint16_t> e_ori_table(
		&Edges::ori_index,
		[](uint16_t i) { return Edges::from_index(0, 0, 0, i); },
		Edges::ori_size
	);
	const AllTwister<Edges, uint8_t> e_slice_prm_table(
		&Edges::slice_prm_index,
		[](uint16_t i) { return Edges::from_index(i / Edges::slice_loc_size, 0, i % Edges::slice_loc_size, 0); },
		Edges::slice_prm_size * Edges::slice_loc_size
	);
	const AllTwister<Edges, uint16_t> e_non_slice_prm_table(
		&Edges::non_slice_prm_index,
		[](uint32_t i) { return Edges::from_index(0, i / Edges::slice_loc_size, i % Edges::slice_loc_size, 0); },
		Edges::non_slice_prm_size * Edges::slice_loc_size
	);
	const AllTwister<Edges, uint16_t> e_slice_loc_table(
		&Edges::slice_loc_index,
		[](uint16_t i) { return Edges::from_index(0, 0, i, 0); },
		Edges::slice_loc_size
	);

	RandomTwistGenerator rnd_twists{ 928322 };
	auto twists = rnd_twists(1'000'000);

	auto e = Edges{};
	auto slice_prm = e.slice_prm_index();
	auto non_slice_prm = e.non_slice_prm_index();
	auto slice_loc = e.slice_loc_index();
	auto ori = e.ori_index();
	for (Twist t : twists)
	{
		e = e.twisted(t);
		slice_prm = e_slice_prm_table(slice_prm * Edges::slice_loc_size + slice_loc, t);
		non_slice_prm = e_non_slice_prm_table(non_slice_prm * Edges::slice_loc_size + slice_loc, t);
		slice_loc = e_slice_loc_table(slice_loc, t);
		ori = e_ori_table(ori, t);
		
		EXPECT_EQ(e.slice_prm_index(), slice_prm);
		EXPECT_EQ(e.non_slice_prm_index(), non_slice_prm);
		EXPECT_EQ(e.slice_loc_index(), slice_loc);
		EXPECT_EQ(e.ori_index(), ori);
	}
}