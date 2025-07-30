#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{
    inline void hash(std::size_t& seed) {}

    template <typename T, typename... Rest>
    void hash(std::size_t& seed, const T& v, Rest... rest)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash(seed, rest...);
    }
}

template <typename T, typename... Rest>
std::size_t hash(const T& v, Rest... rest)
{
    std::size_t seed = 0;
    hash(seed, v, rest...);
    return seed;
}

namespace std
{
	template <typename T>
		requires requires(T t) { t.hash(); }
	struct hash<T>
	{
		size_t operator()(const T& c) const
		{
			return c.hash();
		}
	};
}

template <typename T>
	requires requires(T t) { to_string(t); }
std::ostream& operator<<(std::ostream& os, const T& t)
{
	using std::to_string;
	return os << to_string(t);
}

std::string to_string(std::chrono::nanoseconds);

template <class Sep, class Proj = std::identity>
	requires std::convertible_to<Sep, std::string_view> || std::same_as<Sep, char>
std::string join(Sep separator, const std::ranges::range auto& range, Proj proj = {})
{
	using std::to_string;
	std::string result;
	for (auto it = range.begin(); it != range.end(); ++it)
	{
		if (it != range.begin())
			result += separator;
		result += to_string(proj(*it));
	}
	return result;
}

constexpr uint64_t powi(uint64_t base, uint64_t exp)
{
	uint64_t result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		base *= base;
		exp >>= 1;
	}
	return result;
}

template <typename Key, typename Value>
class HashTable
{
	struct Bucket
	{
		Key key;
		Value value;
	};
	std::vector<Bucket> buckets;
	Bucket sentinel;
public:
	HashTable() = default;
	HashTable(std::size_t size, const Key& sentinel_key, const Value& sentinel_value)
		: buckets(size, { sentinel_key, sentinel_value })
		, sentinel({ sentinel_key, sentinel_value })
	{
	}

	void insert(Key key, Value value)
	{
		std::size_t index = std::hash<Key>{}(key) % buckets.size();
		buckets[index] = { std::move(key), std::move(value) };
	}

	std::optional<Value> retrieve(const Key& key) const
	{
		std::size_t index = std::hash<Key>{}(key) % buckets.size();
		const Bucket& data = buckets[index];
		if (data.key == key)
			return data.value;
		return std::nullopt;
	}

	void clear()
	{
		buckets.assign(buckets.size(), sentinel);
	}
};

template <typename T, std::size_t N>
class InplaceVector
{
	std::size_t size_;
	std::array<T, N> data_;
public:
	InplaceVector() : size_(0) {}

	void push_back(const T& value)
	{
		if (size_ >= N)
			throw std::out_of_range("InplaceVector: exceeded maximum size");
		data_[size_++] = value;
	}

	void insert(std::size_t index, const T& value)
	{
		if (size_ >= N)
			throw std::out_of_range("InplaceVector: exceeded maximum size");
		if (index > size_)
			throw std::out_of_range("InplaceVector: index out of range");
		std::rotate(data_.data() + index, data_.data() + size_, data_.data() + size_ + 1);
		data_[index] = value;
		++size_;
	}

	std::size_t size() const { return size_; }
	bool empty() const { return size_ == 0; }

	auto begin() { return data_.data(); }
	auto begin() const { return data_.data(); }

	auto end() { return data_.data() + size_; }
	auto end() const { return data_.data() + size_; }

	auto data() const { return data_; }

	auto operator[](std::size_t index) { return data_[index]; }
	auto operator[](std::size_t index) const { return data_[index]; }
};
