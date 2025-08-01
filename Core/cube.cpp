#include "cube.h"
#include "twister.h"
#include <limits>

class SliceLocPermutationParity
{
	std::vector<bool> even_perm;
public:
	SliceLocPermutationParity() : even_perm(Edges::slice_loc_size)
	{
		for (int64_t i = 0; i < Edges::slice_loc_size; i++)
		{
			auto e = Edges::from_index(
				Edges{}.slice_prm_index(),
				Edges{}.non_slice_prm_index(),
				static_cast<uint16_t>(i),
				Edges{}.ori_index());
			even_perm[i] = e.is_even_permutation();
		}
	}
	bool operator[](uint16_t i) const { return even_perm[i]; }
};

static const SliceLocPermutationParity slice_loc_parity;

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
	uint16_t c_prm = static_cast<uint16_t>(index % (Corners::prm_size / 2)) * 2;
	index /= Corners::prm_size / 2;
	uint16_t e_non_slice_prm = static_cast<uint16_t>(index % Edges::non_slice_prm_size);
	index /= Edges::non_slice_prm_size;
	uint8_t e_slice_prm = static_cast<uint8_t>(index % Edges::slice_prm_size);

	bool e_even_prm = is_even_permutation(e_non_slice_prm)
		xor is_even_permutation(e_slice_prm)
		xor true; // In subset, e_slice_loc is always an even permutation.
	if (e_even_prm != is_even_permutation(c_prm))
		c_prm += 1;

	constexpr Cube solved{};
	return Cube{
		solved.c_ori,
		c_prm,
		solved.e_ori,
		e_slice_prm,
		e_non_slice_prm,
		solved.e_slice_loc
	};
}

Cube Cube::from_coset(uint64_t number, uint64_t index) noexcept
{
	uint16_t e_slice_loc = static_cast<uint16_t>(number % Edges::slice_loc_size);
	number /= Edges::slice_loc_size;
	uint16_t e_ori = static_cast<uint16_t>(number % Edges::ori_size);
	number /= Edges::ori_size;
	uint16_t c_ori = static_cast<uint16_t>(number);

	uint16_t c_prm = static_cast<uint16_t>(index % (Corners::prm_size / 2)) * 2;
	index /= Corners::prm_size / 2;
	uint16_t e_non_slice_prm = static_cast<uint16_t>(index % Edges::non_slice_prm_size);
	index /= Edges::non_slice_prm_size;
	uint8_t e_slice_prm = static_cast<uint8_t>(index % Edges::slice_prm_size);

	bool e_even_prm = is_even_permutation(e_non_slice_prm)
		xor is_even_permutation(e_slice_prm)
		xor slice_loc_parity[e_slice_loc];
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
	uint64_t index = e_slice_prm;
	index = index * Edges::non_slice_prm_size + e_non_slice_prm;
	index = index * Corners::prm_size / 2 + c_prm / 2;
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
	constexpr Cube solved{};
	return c_ori == solved.c_ori
		and e_ori == solved.e_ori
		and e_slice_loc == solved.e_slice_loc;
}

bool Cube::is_solved() const
{
	constexpr Cube solved{};
	return *this == solved;
}

uint64_t Cube::hash() const
{
	return ::hash(c_ori, c_prm, e_ori, e_slice_prm, e_non_slice_prm, e_slice_loc);
}

std::string to_string(const Cube& c)
{
	return "Cube{"
		+ std::to_string(c.c_ori) + ", "
		+ std::to_string(c.c_prm) + ", "
		+ std::to_string(c.e_ori) + ", "
		+ std::to_string(c.e_slice_prm) + ", "
		+ std::to_string(c.e_non_slice_prm) + ", "
		+ std::to_string(c.e_slice_loc) + "}";
}
