#pragma once
#include "twist.h"
#include "math.h"
#include "std2.h"
#include <array>
#include <cstdint>
#include <string>

class Edges : public Twistable<Edges>
{
	//     +----1----+
	//    /|        /|
	//   4 11      5 10
	//  +----0----+  |
	//  |  |      |  |
	//  |  +----2-|--+
	//  8 /       9 /
	//  |7        |6
	//  +----3----+
	std::array<uint8_t, 12> s = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	Edges(std::array<uint8_t, 12> s) noexcept : s(std::move(s)) {}

	constexpr uint8_t cubie(int) const;
	constexpr uint8_t orientation(int) const;
	std::array<uint8_t, 12> cubies() const;
	std::array<uint8_t, 12> orientations() const;
	Edges(std::array<uint8_t, 12> edges, std::array<uint8_t, 12> orientations) noexcept;
public:
	static constexpr int64_t slice_prm_size = factorial(4);
	static constexpr int64_t non_slice_prm_size = factorial(8);
	static constexpr int64_t slice_loc_size = binomial(12, 4);
	static constexpr int64_t ori_size = powi(2, 11);

	constexpr Edges() noexcept = default;

	auto operator<=>(const Edges&) const = default;

	using Twistable::twisted;
	Edges twisted(Twist) const;

	bool is_solved() const;
	bool is_even_permutation() const;

	static Edges from_index(uint8_t slice_prm, uint16_t non_slice_prm, uint16_t slice_loc, uint16_t ori);
	constexpr uint8_t slice_prm_index() const;
	constexpr uint16_t non_slice_prm_index() const;
	constexpr uint16_t slice_loc_index() const;
	constexpr uint16_t ori_index() const;

	friend std::string to_string(const Edges&);
};


constexpr uint8_t Edges::cubie(int index) const
{
	return s[index] & 0x0F;
}

constexpr uint8_t Edges::orientation(int index) const
{
	return s[index] >> 4;
}

constexpr uint8_t Edges::slice_prm_index() const
{
	std::array<uint8_t, 4> slice;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) > 7)
			slice[j++] = cubie(i) - 8;
	return static_cast<uint8_t>(permutation_index(slice));
}

constexpr uint16_t Edges::non_slice_prm_index() const
{
	std::array<uint8_t, 8> non_slice;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) <= 7)
			non_slice[j++] = cubie(i);
	return static_cast<uint16_t>(permutation_index(non_slice));
}

constexpr uint16_t Edges::ori_index() const
{
	uint16_t ret = 0;
	for (int i = 0; i < 11; i++)
		ret |= static_cast<uint16_t>(orientation(i)) << i;
	return ret;
}

constexpr uint16_t Edges::slice_loc_index() const
{
	std::array<uint8_t, 4> loc;
	for (int i = 0, j = 0; i < 12; i++)
		if (cubie(i) > 7)
			loc[j++] = i;
	return static_cast<uint16_t>(combination_index(12, loc));
}