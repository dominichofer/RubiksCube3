#pragma once
#include "std2.h"
#include <bit>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

enum class Twist : uint8_t
{
	L1, L2, L3,
	R1, R2, R3,
	U1, U2, U3,
	D1, D2, D3,
	F1, F2, F3,
	B1, B2, B3,
	None
};

namespace twist
{
	class Iterator
	{
		uint32_t twists{ 0 };
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = Twist;
		using reference = Twist&;
		using pointer = Twist*;
		using iterator_category = std::forward_iterator_tag;

		Iterator() noexcept = default;
		Iterator(uint32_t twists) noexcept : twists(twists) {}

		auto operator<=>(const Iterator&) const = default;

		Iterator& operator++() noexcept { ClearLSB(twists); return *this; }
		Twist operator*() const noexcept { return static_cast<Twist>(std::countr_zero(twists)); }
	};
}

class Twists
{
	uint32_t b{ 0 };
public:
	Twists() noexcept = default;
	explicit Twists(uint32_t twists) noexcept : b(twists) {}

	static Twists all() noexcept { return Twists{ 0b111'111'111'111'111'111 }; }
	// H0 = { L2,R2,U,D,F2,B2 }
	static Twists H0() noexcept { return Twists{ 0b010'010'111'111'010'010 }; }

	auto operator<=>(const Twists&) const = default;
	Twists operator~() const { return Twists{ b ^ all().b }; }

	Twist operator[](std::size_t index) const noexcept { return static_cast<Twist>(std::countr_zero(PDep(1 << index, b))); }

	bool contains(Twist t) const noexcept { return b & (1 << std::to_underlying(t)) != 0; }
	void erase_face(Twist face);
	void remove(Twists t) { b &= ~t.b; }
	void keep(Twists t) { b &= t.b; }
	std::size_t size() const noexcept { return std::popcount(b); }

	twist::Iterator begin() const noexcept { return b; }
	twist::Iterator end() const noexcept { return {}; }
};

std::string to_string(Twist);
std::string to_string(const std::vector<Twist>&);

auto twist_from_string(std::string_view) -> Twist;
auto twists_from_string(std::string_view) -> std::vector<Twist>;

Twist inversed(Twist);
std::vector<Twist> inversed(std::vector<Twist>);
void inverse(std::vector<Twist>&);

template <typename Derived>
class Twistable
{
public:
	auto operator<=>(const Twistable&) const = default;

	virtual Derived twisted(Twist) const = 0;

	Derived twisted(const std::vector<Twist>& twists) const
	{
		auto cube = static_cast<const Derived&>(*this);
		for (Twist t : twists)
			cube = cube.twisted(t);
		return cube;
	}
};
