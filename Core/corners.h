#pragma once
#include "twist.h"
#include "math.h"
#include "std2.h"
#include <array>
#include <cstdint>
#include <string>

class Corners : public Twistable<Corners>
{
	//     2---------3
	//    /|        /|
	//   / |       / |
	//  0---------1  |
	//  |  |      |  |
	//  |  6------|--7
	//  | /       | /
	//  |/        |/
	//  4---------5
	std::array<uint8_t, 8> s = { 0, 1, 2, 3, 4, 5, 6, 7 };

	Corners(std::array<uint8_t, 8> s) noexcept : s(std::move(s)) {}

	uint8_t cubie(int) const;
	uint8_t orientation(int) const;
	std::array<uint8_t, 8> cubies() const;
	std::array<uint8_t, 8> orientations() const;
	Corners(std::array<uint8_t, 8> cubies, std::array<uint8_t, 8> orientations) noexcept;
public:
	static constexpr uint16_t prm_size = factorial(8);
	static constexpr uint16_t ori_size = powi(3, 7);
	static constexpr uint32_t index_size = prm_size * ori_size;

	constexpr Corners() noexcept = default;

	auto operator<=>(const Corners&) const = default;

	using Twistable::twisted;
	Corners twisted(Twist) const;

	bool is_solved() const;

	static Corners from_index(uint16_t prm, uint16_t ori);
	constexpr uint16_t prm_index() const;
	constexpr uint16_t ori_index() const;

	static Corners from_index(uint32_t);
	uint32_t index() const;

	friend std::string to_string(const Corners&);
};
