#include "pch.h"

using namespace std::chrono_literals;

TEST(hash, one_arg) { EXPECT_NE(hash(0), hash(1)); }
TEST(hash, two_args) { EXPECT_NE(hash(0, 1), hash(1, 0)); }
TEST(hash, three_args) { EXPECT_NE(hash(0, 1, 2), hash(2, 1, 0)); }

TEST(to_string, 1ns) { EXPECT_EQ(to_string(1ns), "1.00 ns"); }
TEST(to_string, 10ns) { EXPECT_EQ(to_string(10ns), "10.0 ns"); }
TEST(to_string, 100ns) { EXPECT_EQ(to_string(100ns), "100 ns"); }
TEST(to_string, 1us) { EXPECT_EQ(to_string(1us), "1.00 us"); }
TEST(to_string, 10us) { EXPECT_EQ(to_string(10us), "10.0 us"); }
TEST(to_string, 100us) { EXPECT_EQ(to_string(100us), "100 us"); }
TEST(to_string, 1ms) { EXPECT_EQ(to_string(1ms), "1.00 ms"); }
TEST(to_string, 10ms) { EXPECT_EQ(to_string(10ms), "10.0 ms"); }
TEST(to_string, 100ms) { EXPECT_EQ(to_string(100ms), "100 ms"); }
TEST(to_string, 1s) { EXPECT_EQ(to_string(1s), "1.00 s"); }
TEST(to_string, 10s) { EXPECT_EQ(to_string(10s), "10.0 s"); }
TEST(to_string, 100s) { EXPECT_EQ(to_string(100s), "100 s"); }
TEST(to_string, 1000s) { EXPECT_EQ(to_string(1000s), "1000 s"); }

TEST(join, empty) { EXPECT_EQ(join(",", std::vector<int>{}), ""); }
TEST(join, one_element) { EXPECT_EQ(join(",", std::vector<int>{ 1 }), "1"); }
TEST(join, two_elements) { EXPECT_EQ(join(",", std::vector<int>{ 1, 2 }), "1,2"); }
TEST(join, with_projection) { EXPECT_EQ(join("-", std::vector<int>{ 1, 2, 3 }, [](int i) { return i * 10; }), "10-20-30"); }

TEST(join, char_separator) { EXPECT_EQ(join(',', std::vector<int>{ 1, 2 }), "1,2"); }
TEST(join, char_arr_separator) { EXPECT_EQ(join(",", std::vector<int>{ 1, 2 }), "1,2"); }
TEST(join, string_separator) { EXPECT_EQ(join(std::string(","), std::vector<int>{ 1, 2 }), "1,2"); }
TEST(join, string_view_separator) { EXPECT_EQ(join(std::string_view(","), std::vector<int>{ 1, 2 }), "1,2"); }


TEST(powi, small_numbers)
{
	EXPECT_EQ(powi(2, 3), 8);
	EXPECT_EQ(powi(3, 2), 9);
	EXPECT_EQ(powi(5, 4), 625);
}

TEST(powi, zero_exponent)
{
	EXPECT_EQ(powi(2, 0), 1);
	EXPECT_EQ(powi(0, 0), 1); // By convention, 0^0 = 1
	EXPECT_EQ(powi(12345, 0), 1);
}

TEST(powi, zero_base)
{
	EXPECT_EQ(powi(0, 1), 0);
	EXPECT_EQ(powi(0, 5), 0);
}

TEST(powi, one_base)
{
	EXPECT_EQ(powi(1, 100), 1);
	EXPECT_EQ(powi(1, 0), 1);
}

TEST(powi, edge_cases)
{
	EXPECT_EQ(powi(UINT64_MAX, 1), UINT64_MAX);
	EXPECT_EQ(powi(UINT64_MAX, 0), 1);
}

TEST(HashTable, states)
{
	HashTable<int, int> ht{ /*size*/ 100, /*sentinel_key*/ 0, /*sentinel_value*/ 0 };
	const int key = 13; // arbitrary
	const int value1 = 14; // arbitrary
	const int value2 = 15; // arbitrary

	// key is not in table
	EXPECT_EQ(ht.retrieve(key), std::nullopt);

	// inserting a value makes it contained
	ht.insert(key, value1);
	EXPECT_EQ(ht.retrieve(key), value1);

	// overwriting a value makes it contained
	ht.insert(key, value2);
	EXPECT_EQ(ht.retrieve(key), value2);

	// clearing removes key
	ht.clear();
	EXPECT_EQ(ht.retrieve(key), std::nullopt);
}

TEST(InplaceVector, states)
{
	InplaceVector<int, 3> vec;
	const int value1 = 14; // arbitrary
	const int value2 = 15; // arbitrary
	const int value3 = 16; // arbitrary

	// Initially empty
	EXPECT_EQ(vec.size(), 0);
	EXPECT_TRUE(vec.empty());

	// Pushing back a value makes it contained
	vec.push_back(value1);
	EXPECT_FALSE(vec.empty());

	vec.push_back(value3);

	// Inserting a value in the middle
	vec.insert(1, value2);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec.data(), (std::array<int, 3>{value1, value2, value3}));
}
