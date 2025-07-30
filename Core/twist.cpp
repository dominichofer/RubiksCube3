#include "twist.h"
#include "std2.h"
#include <stdexcept>

std::string to_string(Twist twist)
{
	switch (twist)
	{
	case Twist::L1: return "L1";
	case Twist::L2: return "L2";
	case Twist::L3: return "L3";
	case Twist::R1: return "R1";
	case Twist::R2: return "R2";
	case Twist::R3: return "R3";
	case Twist::U1: return "U1";
	case Twist::U2: return "U2";
	case Twist::U3: return "U3";
	case Twist::D1: return "D1";
	case Twist::D2: return "D2";
	case Twist::D3: return "D3";
	case Twist::F1: return "F1";
	case Twist::F2: return "F2";
	case Twist::F3: return "F3";
	case Twist::B1: return "B1";
	case Twist::B2: return "B2";
	case Twist::B3: return "B3";
	case Twist::None: return "None";
	default: throw std::invalid_argument("Invalid twist");
	}
}

std::string to_string(const Twists& t)
{
	return join(' ', t);
}

Twist twist_from_string(std::string_view s)
{
	for (Twist t : all_twists)
		if (to_string(t) == s)
			return t;
	throw std::invalid_argument("Invalid twist string");
}

Twists twists_from_string(std::string_view s)
{
	Twists ret;
	for (size_t i = 0; i < s.size(); i += 3)
		ret.push_back(twist_from_string(s.substr(i, 2)));
	return ret;
}

Twist inversed(Twist twist)
{
	switch (twist)
	{
	case Twist::L1: return Twist::L3;
	case Twist::L2: return Twist::L2;
	case Twist::L3: return Twist::L1;
	case Twist::R1: return Twist::R3;
	case Twist::R2: return Twist::R2;
	case Twist::R3: return Twist::R1;
	case Twist::U1: return Twist::U3;
	case Twist::U2: return Twist::U2;
	case Twist::U3: return Twist::U1;
	case Twist::D1: return Twist::D3;
	case Twist::D2: return Twist::D2;
	case Twist::D3: return Twist::D1;
	case Twist::F1: return Twist::F3;
	case Twist::F2: return Twist::F2;
	case Twist::F3: return Twist::F1;
	case Twist::B1: return Twist::B3;
	case Twist::B2: return Twist::B2;
	case Twist::B3: return Twist::B1;
	case Twist::None: return Twist::None;
	default: throw std::invalid_argument("Invalid twist");
	}
}

Twists inversed(Twists t)
{
	inverse(t);
	return t;
}

void inverse(Twists& t)
{
	for (int8_t i = 0; i < t.size() / 2; ++i)
		std::swap(t[i], t[t.size() - i - 1]);
	for (int8_t i = 0; i < t.size(); ++i)
		t[i] = inversed(t[i]);
}

bool is_same_face(Twist a, Twist b)
{
	return std::to_underlying(a) / 3 == std::to_underlying(b) / 3;
}
