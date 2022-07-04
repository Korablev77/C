#pragma once

#include <cstring>

#include "Intr.h"

// Treat string as a pair of pointer and len.
// We could use std::strings but it would lead to unnecessary allocations
// (even taking in account small buffer optimization which is copy anyway).
// std::string_view perhaps performs not much worse than this one, but I
// haven't tested it.
struct LightStr
{
	char *ptr;
	std::size_t len;
	std::size_t hash;
	explicit LightStr(char *p, std::size_t l, std::size_t h) : ptr(p), len(l), hash(h) { }

	~LightStr() { }

	LightStr(const LightStr&) = default;
	LightStr(LightStr &&) noexcept = default;
	LightStr& operator=(const LightStr&) = default;
	LightStr& operator=(LightStr&&) = default;
};

namespace Hash {
	static constexpr std::uint32_t offset_basis = 0x811c9dc5;
	static constexpr std::uint32_t prime = 0x01000193;
	// Unfortunately standard library does not provide hash function for raw bytes.
	// So let's write own simple implementation based on Fowler–Noll–Vo algorithm,
	// it's just taken from Wikipedia :)). Have no idea how good is it in fact.
	// But at this point we should not much care about solid hash function.
	// Hash calculation takes place in words_dict_create_from_raw() right
	// alongside with memory traversal so that avoid any extra passes.
	struct RawBytes
	{
		std::size_t operator()(const LightStr &str) const
		{
			return str.hash;
		}
	};
};

// Compare two NOT NULL terminated strings with possibly different lengths.
inline int
raw_bytes_cmp(char *lhs, std::size_t lhs_len, char *rhs, std::size_t rhs_len)
{
	std::size_t len = std::min(lhs_len, rhs_len);
	int rc = my_memcmp(lhs, rhs, len);
	if (rc != 0)
		return rc;

	if (lhs_len == rhs_len)
		return 0;
	return lhs_len < rhs_len ? -1 : 1;
}

// unordered_map also requires equality check function in case hashes collide.
struct RawBytesEqual
{
	bool operator()(const LightStr &lhs, const LightStr &rhs) const
	{
		return raw_bytes_cmp(lhs.ptr, lhs.len, rhs.ptr, rhs.len) == 0;
	}
};