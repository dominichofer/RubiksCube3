#pragma once
#include "corners.h"
#include "edges.h"
#include "twist.h"
#include <cstdint>

class Cube : public Twistable<Cube>
{
	Cube(
		uint16_t c_ori,
		uint16_t c_prm,
		uint16_t e_ori,
		uint8_t e_slice_prm,
		uint16_t e_non_slice_prm,
		uint16_t e_slice_loc) noexcept;
public:
	uint16_t c_ori; // 3^7 = 2'187
	uint16_t c_prm; // 8! = 40'320
	uint16_t e_ori; // 2^11 = 2'048
	uint8_t e_slice_prm; // 4! = 24
	uint16_t e_non_slice_prm; // 8! = 40'320
	uint16_t e_slice_loc; // 12 choose 4 = 495

	static constexpr uint64_t set_size = Corners::prm_size / 2 * Edges::slice_prm_size * Edges::non_slice_prm_size;
	static constexpr uint32_t cosets = Corners::ori_size * Edges::ori_size * Edges::slice_loc_size;

	constexpr Cube() noexcept;
	static Cube impossible() noexcept;

	auto operator<=>(const Cube&) const = default;

	using Twistable::twisted;
	Cube twisted(Twist) const;

	static Cube from_subset(uint64_t) noexcept;
	static Cube from_coset(uint64_t number, uint64_t index) noexcept;
	uint64_t coset_index() const;
	uint64_t coset_number() const;

	bool in_subset() const;
	bool is_solved() const;

	uint64_t hash() const;
};
