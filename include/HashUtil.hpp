#ifndef HASHUTIL_HPP
#define HASHUTIL_HPP

#include <string>
#include <sstream>
#include <iomanip>

inline std::string simpleHash(const std::string& input) {
    uint64_t hash = 14695981039346656037ull; // FNV-1a offset basis
    for (char c : input) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 1099511628211ull; // FNV-1a prime
    }

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

#endif // HASHUTIL_HPP
