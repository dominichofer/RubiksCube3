#include "edges.h"

static std::array<uint8_t, 12> ori_swap_L(std::array<uint8_t, 12> s)
{
	for (int i : { 4, 7, 8, 11 })
		s[i] ^= 0x10;
	return s;
}

static std::array<uint8_t, 12> ori_swap_R(std::array<uint8_t, 12> s)
{
	for (int i : { 5, 6, 9, 10 })
		s[i] ^= 0x10;
	return s;
}

static std::array<uint8_t, 12> shuffled(const std::array<uint8_t, 12>& s, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l)
{
	return { s[a], s[b], s[c], s[d], s[e], s[f], s[g], s[h], s[i], s[j], s[k], s[l] };
}

uint8_t Edges::cubie(int index) const
{
	return s[index] & 0x0F;
}

uint8_t Edges::orientation(int index) const
{
	return s[index] >> 4;
}

std::array<uint8_t, 12> Edges::cubies() const
{
	return {
		cubie(0), cubie(1), cubie(2), cubie(3),
		cubie(4), cubie(5), cubie(6), cubie(7),
		cubie(8), cubie(9), cubie(10), cubie(11)
	};
}

std::array<uint8_t, 12> Edges::orientations() const
{
	return {
		orientation(0), orientation(1), orientation(2), orientation(3),
		orientation(4), orientation(5), orientation(6), orientation(7),
		orientation(8), orientation(9), orientation(10), orientation(11)
	};
}

Edges::Edges(std::array<uint8_t, 12> edges, std::array<uint8_t, 12> orientations) noexcept
{
	for (int i = 0; i < 12; i++)
		s[i] = (orientations[i] << 4) | edges[i];
}

Edges Edges::twisted(Twist r) const
{
	switch (r)
	{
	case Twist::L1:
		return ori_swap_L(shuffled(s, 0, 1, 2, 3, 11, 5, 6, 8, 4, 9, 10, 7));
	case Twist::L2:
		return shuffled(s, 0, 1, 2, 3, 7, 5, 6, 4, 11, 9, 10, 8);
	case Twist::L3:
		return ori_swap_L(shuffled(s, 0, 1, 2, 3, 8, 5, 6, 11, 7, 9, 10, 4));
	case Twist::R1:
		return ori_swap_R(shuffled(s, 0, 1, 2, 3, 4, 9, 10, 7, 8, 6, 5, 11));
	case Twist::R2:
		return shuffled(s, 0, 1, 2, 3, 4, 6, 5, 7, 8, 10, 9, 11);
	case Twist::R3:
		return ori_swap_R(shuffled(s, 0, 1, 2, 3, 4, 10, 9, 7, 8, 5, 6, 11));
	case Twist::U1:
		return shuffled(s, 5, 4, 2, 3, 0, 1, 6, 7, 8, 9, 10, 11);
	case Twist::U2:
		return shuffled(s, 1, 0, 2, 3, 5, 4, 6, 7, 8, 9, 10, 11);
	case Twist::U3:
		return shuffled(s, 4, 5, 2, 3, 1, 0, 6, 7, 8, 9, 10, 11);
	case Twist::D1:
		return shuffled(s, 0, 1, 6, 7, 4, 5, 3, 2, 8, 9, 10, 11);
	case Twist::D2:
		return shuffled(s, 0, 1, 3, 2, 4, 5, 7, 6, 8, 9, 10, 11);
	case Twist::D3:
		return shuffled(s, 0, 1, 7, 6, 4, 5, 2, 3, 8, 9, 10, 11);
	case Twist::F1:
		return shuffled(s, 8, 1, 2, 9, 4, 5, 6, 7, 3, 0, 10, 11);
	case Twist::F2:
		return shuffled(s, 3, 1, 2, 0, 4, 5, 6, 7, 9, 8, 10, 11);
	case Twist::F3:
		return shuffled(s, 9, 1, 2, 8, 4, 5, 6, 7, 0, 3, 10, 11);
	case Twist::B1:
		return shuffled(s, 0, 10, 11, 3, 4, 5, 6, 7, 8, 9, 2, 1);
	case Twist::B2:
		return shuffled(s, 0, 2, 1, 3, 4, 5, 6, 7, 8, 9, 11, 10);
	case Twist::B3:
		return shuffled(s, 0, 11, 10, 3, 4, 5, 6, 7, 8, 9, 1, 2);
	default:
		return *this;
	}
}

bool Edges::is_solved() const
{
	return *this == Edges{};
}

Edges Edges::from_index(uint8_t slice_prm, uint16_t non_slice_prm, uint16_t slice_loc_index, uint16_t ori)
{
	std::array<uint8_t, 12> e;
	nth_permutation(non_slice_prm, e.begin(), 8);

	nth_permutation(slice_prm, e.begin() + 8, 4);
	for (int i = 8; i < 12; i++)
		e[i] += 8; // slice cubies are indexed from 8 to 11

	std::array<uint8_t, 4> slice_loc;
	nth_combination(12, 4, slice_loc_index, slice_loc);

	for (int i = 0; i < 4; i++)
	{
		auto begin = e.rbegin() + 3 - i;
		auto end = e.rbegin() + 12 - slice_loc[i];
		std::rotate(begin, begin + 1, end);
	}

	std::array<uint8_t, 12> o;
	for (int i = 0; i < 11; i++)
		o[i] = (ori >> i) & 1;
	o[11] = std::popcount(ori) % 2; // parity of the number of 1s in the orientation index

	return Edges{ e, o };
}

uint8_t Edges::slice_prm_index() const
{
	std::array<uint8_t, 4> slice;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) > 7)
			slice[j++] = cubie(i) - 8;
	return static_cast<uint8_t>(permutation_index(slice));
}

uint16_t Edges::non_slice_prm_index() const
{
	std::array<uint8_t, 8> non_slice;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) <= 7)
			non_slice[j++] = cubie(i);
	return static_cast<uint16_t>(permutation_index(non_slice));
}

uint16_t Edges::ori_index() const
{
	uint16_t ret = 0;
	for (int i = 0; i < 11; i++)
		ret |= static_cast<uint16_t>(orientation(i)) << i;
	return ret;
}

uint16_t Edges::slice_loc_index() const
{
	std::array<uint8_t, 4> loc;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) > 7)
			loc[j++] = i;
	return static_cast<uint16_t>(combination_index(12, loc));
}

std::string to_string(const Edges& e)
{
	return join(' ', e.cubies()) + " | " + join(' ', e.orientations());
}
