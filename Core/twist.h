#pragma once
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

using Twists = std::vector<Twist>;

std::string to_string(Twist);
std::string to_string(const Twists&);

auto twist_from_string(std::string_view) -> Twist;
auto twists_from_string(std::string_view) -> Twists;

static inline const Twists all_twists = {
	Twist::L1, Twist::L2, Twist::L3,
	Twist::R1, Twist::R2, Twist::R3,
	Twist::U1, Twist::U2, Twist::U3,
	Twist::D1, Twist::D2, Twist::D3,
	Twist::F1, Twist::F2, Twist::F3,
	Twist::B1, Twist::B2, Twist::B3
};

// H0 = { L2,R2,U,D,F2,B2 }
namespace H0
{
	static inline const Twists twists = {
			Twist::L2,
			Twist::R2,
			Twist::U1, Twist::U2, Twist::U3,
			Twist::D1, Twist::D2, Twist::D3,
			Twist::F2,
			Twist::B2,
	};
	static inline const Twists non_twists = {
			Twist::L1, Twist::L3,
			Twist::R1, Twist::R3,
			Twist::F1, Twist::F3,
			Twist::B1, Twist::B3,
	};
}

Twist inversed(Twist);
Twists inversed(Twists);
void inverse(Twists&);

bool is_same_face(Twist, Twist);

template <typename Derived>
class Twistable
{
public:
	auto operator<=>(const Twistable&) const = default;

	virtual Derived twisted(Twist) const = 0;

	Derived twisted(const Twists& twists) const
	{
		auto cube = static_cast<const Derived&>(*this);
		for (Twist t : twists)
			cube = cube.twisted(t);
		return cube;
	}
};
