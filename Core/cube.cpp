#include "cube.h"
#include "twister.h"
#include <limits>

static const AllTwister<Corners, uint16_t> c_ori_table(
	&Corners::ori_index,
	[](uint16_t i) { return Corners::from_index(0, i); },
	Corners::ori_size
);

static const AllTwister<Corners, uint16_t> c_prm_table(
	&Corners::prm_index,
	[](uint16_t i) { return Corners::from_index(i, 0); },
	Corners::prm_size
);

static const AllTwister<Edges, uint16_t> e_ori_table(
	&Edges::ori_index,
	[](uint16_t i) { return Edges::from_index(0, 0, 0, i); },
	Edges::ori_size
);

static const AllTwister<Edges, uint8_t> e_slice_prm_table(
	&Edges::slice_prm_index,
	[](uint16_t i) { return Edges::from_index(i / Edges::slice_loc_size, 0, i % Edges::slice_loc_size, 0); },
	Edges::slice_prm_size* Edges::slice_loc_size
);

static const AllTwister<Edges, uint16_t> e_non_slice_prm_table(
	&Edges::non_slice_prm_index,
	[](uint32_t i) { return Edges::from_index(0, i / Edges::slice_loc_size, i % Edges::slice_loc_size, 0); },
	Edges::non_slice_prm_size* Edges::slice_loc_size
);

static const AllTwister<Edges, uint16_t> e_slice_loc_table(
	&Edges::slice_loc_index,
	[](uint16_t i) { return Edges::from_index(0, 0, i, 0); },
	Edges::slice_loc_size
);

Cube::Cube(
	uint16_t c_ori,
	uint16_t c_prm,
	uint16_t e_ori,
	uint8_t e_slice_prm,
	uint16_t e_non_slice_prm,
	uint16_t e_slice_loc) noexcept
	: c_ori(c_ori)
	, c_prm(c_prm)
	, e_ori(e_ori)
	, e_slice_prm(e_slice_prm)
	, e_non_slice_prm(e_non_slice_prm)
	, e_slice_loc(e_slice_loc)
{
}

Cube::Cube() noexcept
	: Cube(
		Corners{}.ori_index(),
		Corners{}.prm_index(),
		Edges{}.ori_index(),
		Edges{}.slice_prm_index(),
		Edges{}.non_slice_prm_index(),
		Edges{}.slice_loc_index())
{
}

Cube Cube::impossible() noexcept
{
	return Cube{
		std::numeric_limits<uint16_t>::max(),
		std::numeric_limits<uint16_t>::max(),
		std::numeric_limits<uint16_t>::max(),
		std::numeric_limits<uint8_t>::max(),
		std::numeric_limits<uint16_t>::max(),
		std::numeric_limits<uint16_t>::max()
	};
}

Cube Cube::twisted(Twist t) const
{
	return Cube{
		c_ori_table(c_ori, t),
		c_prm_table(c_prm, t),
		e_ori_table(e_ori, t),
		e_slice_prm_table(e_slice_prm * Edges::slice_loc_size + e_slice_loc, t),
		e_non_slice_prm_table(e_non_slice_prm * Edges::slice_loc_size + e_slice_loc, t),
		e_slice_loc_table(e_slice_loc, t)
	};
}

Cube Cube::from_subset(uint64_t index) noexcept
{
	uint16_t e_non_slice_prm = static_cast<uint16_t>(index % Edges::non_slice_prm_size);
	index /= Edges::non_slice_prm_size;
	uint8_t e_slice_prm = static_cast<uint8_t>(index % Edges::slice_prm_size);
	index /= Edges::slice_prm_size;
	uint16_t c_prm = static_cast<uint16_t>(index);

	bool e_even_prm = is_even_permutation(e_non_slice_prm) xor is_even_permutation(e_slice_prm);
	if (e_even_prm != is_even_permutation(c_prm))
		c_prm += 1;

	return Cube{
		Corners{}.ori_index(),
		c_prm,
		Edges{}.ori_index(),
		e_slice_prm,
		e_non_slice_prm,
		Edges{}.slice_loc_index()
	};
}

Cube Cube::from_coset(uint64_t number, uint64_t index) noexcept
{
	uint16_t e_slice_loc = static_cast<uint16_t>(number % Edges::slice_loc_size);
	number /= Edges::slice_loc_size;
	uint16_t e_ori = static_cast<uint16_t>(number % Edges::ori_size);
	number /= Edges::ori_size;
	uint16_t c_ori = static_cast<uint16_t>(number);

	uint16_t e_non_slice_prm = static_cast<uint16_t>(index % Edges::non_slice_prm_size);
	index /= Edges::non_slice_prm_size;
	uint8_t e_slice_prm = static_cast<uint8_t>(index % Edges::slice_prm_size);
	index /= Edges::slice_prm_size;
	uint16_t c_prm = static_cast<uint16_t>(index);

	bool e_even_prm = is_even_permutation(e_non_slice_prm) xor is_even_permutation(e_slice_prm); // TODO: Add e_slice_loc!
	if (e_even_prm != is_even_permutation(c_prm))
		c_prm += 1;

	return Cube{
		c_ori,
		c_prm,
		e_ori,
		e_slice_prm,
		e_non_slice_prm,
		e_slice_loc
	};
}

uint64_t Cube::coset_index() const
{
	uint64_t index = c_prm / 2;
	index = index * Edges::slice_prm_size + e_slice_prm;
	index = index * Edges::non_slice_prm_size + e_non_slice_prm;
	return index;
}

uint64_t Cube::coset_number() const
{
	uint32_t number = c_ori;
	number = number * Edges::ori_size + e_ori;
	number = number * Edges::slice_loc_size + e_slice_loc;
	return number;
}

bool Cube::in_subset() const
{
	return c_ori == Corners{}.ori_index()
		and e_ori == Edges{}.ori_index()
		and e_slice_loc == Edges{}.slice_loc_index();
}

bool Cube::is_solved() const
{
	return c_ori == Corners{}.ori_index()
		and c_prm == Corners{}.prm_index()
		and e_ori == Edges{}.ori_index()
		and e_slice_prm == Edges{}.slice_prm_index()
		and e_non_slice_prm == Edges{}.non_slice_prm_index()
		and e_slice_loc == Edges{}.slice_loc_index();
}

uint64_t Cube::hash() const
{
	return ::hash(c_ori, c_prm, e_ori, e_slice_prm, e_non_slice_prm, e_slice_loc);
}
