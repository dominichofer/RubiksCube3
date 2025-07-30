#include "corners.h"

static uint8_t ori_swap_0_1(uint8_t state)
{
	return ((~state & 0x20) >> 1) ^ state;
}

static uint8_t ori_swap_0_2(uint8_t state)
{
	return (0x20 - (state & 0x30)) | (state & 0x0F);
}

static uint8_t ori_swap_1_2(uint8_t state)
{
	uint8_t l = (state & 0x20) >> 1;
	uint8_t r = (state & 0x10) << 1;
	return (state & 0x0F) | l | r;
}

static std::array<uint8_t, 8> ori_swap_L(std::array<uint8_t, 8> s)
{
	for (int i : { 0, 2, 4, 6})
		s[i] = ori_swap_0_2(s[i]);
	return s;
}

static std::array<uint8_t, 8> ori_swap_R(std::array<uint8_t, 8> s)
{
	for (int i : { 1, 3, 5, 7 })
		s[i] = ori_swap_0_2(s[i]);
	return s;
}

static std::array<uint8_t, 8> ori_swap_U(std::array<uint8_t, 8> s)
{
	for (int i : { 0, 1, 2, 3 })
		s[i] = ori_swap_1_2(s[i]);
	return s;
}

static std::array<uint8_t, 8> ori_swap_D(std::array<uint8_t, 8> s)
{
	for (int i : { 4, 5, 6, 7})
		s[i] = ori_swap_1_2(s[i]);
	return s;
}

static std::array<uint8_t, 8> ori_swap_F(std::array<uint8_t, 8> s)
{
	for (int i : { 0, 1, 4, 5 })
		s[i] = ori_swap_0_1(s[i]);
	return s;
}

static std::array<uint8_t, 8> ori_swap_B(std::array<uint8_t, 8> s)
{
	for (int i : { 2, 3, 6, 7 })
		s[i] = ori_swap_0_1(s[i]);
	return s;
}

static std::array<uint8_t, 8> shuffled(const std::array<uint8_t, 8>& s, int a, int b, int c, int d, int e, int f, int g, int h)
{
	return { s[a], s[b], s[c], s[d], s[e], s[f], s[g], s[h] };
}

uint8_t Corners::cubie(int index) const
{
	return s[index] & 0x0F;
}

uint8_t Corners::orientation(int index) const
{
	return s[index] >> 4;
}

std::array<uint8_t, 8> Corners::cubies() const
{
	std::array<uint8_t, 8> ret;
	for (int i = 0; i < 8; i++)
		ret[i] = cubie(i);
	return ret;
}

std::array<uint8_t, 8> Corners::orientations() const
{
	std::array<uint8_t, 8> ret;
	for (int i = 0; i < 8; i++)
		ret[i] = orientation(i);
	return ret;
}

Corners::Corners(std::array<uint8_t, 8> corners, std::array<uint8_t, 8> orientations) noexcept
{
	for (int i = 0; i < 8; i++)
		s[i] = (orientations[i] << 4) | corners[i];
}

Corners Corners::twisted(Twist r) const
{
	switch (r)
	{
	case Twist::L1:
		return ori_swap_L(shuffled(s, 2, 1, 6, 3, 0, 5, 4, 7));
	case Twist::L2:
		return shuffled(s, 6, 1, 4, 3, 2, 5, 0, 7);
	case Twist::L3:
		return ori_swap_L(shuffled(s, 4, 1, 0, 3, 6, 5, 2, 7));
	case Twist::R1:
		return ori_swap_R(shuffled(s, 0, 5, 2, 1, 4, 7, 6, 3));
	case Twist::R2:
		return shuffled(s, 0, 7, 2, 5, 4, 3, 6, 1);
	case Twist::R3:
		return ori_swap_R(shuffled(s, 0, 3, 2, 7, 4, 1, 6, 5));
	case Twist::U1:
		return ori_swap_U(shuffled(s, 1, 3, 0, 2, 4, 5, 6, 7));
	case Twist::U2:
		return shuffled(s, 3, 2, 1, 0, 4, 5, 6, 7);
	case Twist::U3:
		return ori_swap_U(shuffled(s, 2, 0, 3, 1, 4, 5, 6, 7));
	case Twist::D1:
		return ori_swap_D(shuffled(s, 0, 1, 2, 3, 6, 4, 7, 5));
	case Twist::D2:
		return shuffled(s, 0, 1, 2, 3, 7, 6, 5, 4);
	case Twist::D3:
		return ori_swap_D(shuffled(s, 0, 1, 2, 3, 5, 7, 4, 6));
	case Twist::F1:
		return ori_swap_F(shuffled(s, 4, 0, 2, 3, 5, 1, 6, 7));
	case Twist::F2:
		return shuffled(s, 5, 4, 2, 3, 1, 0, 6, 7);
	case Twist::F3:
		return ori_swap_F(shuffled(s, 1, 5, 2, 3, 0, 4, 6, 7));
	case Twist::B1:
		return ori_swap_B(shuffled(s, 0, 1, 3, 7, 4, 5, 2, 6));
	case Twist::B2:
		return shuffled(s, 0, 1, 7, 6, 4, 5, 3, 2);
	case Twist::B3:
		return ori_swap_B(shuffled(s, 0, 1, 6, 2, 4, 5, 7, 3));
	default:
		return *this;
	}
}

bool Corners::is_solved() const
{
	return *this == Corners{};
}

Corners Corners::from_index(uint16_t prm, uint16_t ori)
{
	std::array<uint8_t, 8> c;
	nth_permutation(prm, c);

	uint8_t o6 = ori % 3; ori /= 3;
	uint8_t o5 = ori % 3; ori /= 3;
	uint8_t o4 = ori % 3; ori /= 3;
	uint8_t o3 = ori % 3; ori /= 3;
	uint8_t o2 = ori % 3; ori /= 3;
	uint8_t o1 = ori % 3; ori /= 3;
	uint8_t o0 = ori % 3; ori /= 3;
	uint8_t o7 = (12 + o0 - o1 - o2 + o3 - o4 + o5 + o6) % 3;
	return { c, { o0, o1, o2, o3, o4, o5, o6, o7 } };
}

uint16_t Corners::prm_index() const
{
	return static_cast<uint16_t>(permutation_index(cubies()));
}

uint16_t Corners::ori_index() const
{
	uint16_t ret = 0;
	for (int i = 0; i < 7; i++)
	{
		ret *= 3;
		ret += orientation(i);
	}
	return ret;
}

Corners Corners::from_index(uint32_t index)
{
	return from_index(
		static_cast<uint16_t>(index / ori_size),
		static_cast<uint16_t>(index % ori_size)
	);
}

uint32_t Corners::index() const
{
	return static_cast<uint32_t>(prm_index() * ori_size + ori_index());
}

std::string to_string(const Corners& c)
{
	return join(' ', c.cubies()) + " | " + join(' ', c.orientations());
}
