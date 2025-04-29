// this file is a utility header file that provides a simple hash function for strings using the FNV-1a algorithm.

#ifndef HASHUTIL_HPP
#define HASHUTIL_HPP

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

inline string simpleHash(const string& input) {
    uint64_t hash = 14695981039346656037ull;
    for (char c : input) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 1099511628211ull;
    }

    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

#endif // HASHUTIL_HPP
