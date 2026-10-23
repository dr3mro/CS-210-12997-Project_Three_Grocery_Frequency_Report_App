/**
 * @file fuzz_serializer.cpp
 * @brief libFuzzer harness for the grocery Serializer.
 *
 * Feeds fuzzed bytes into Serializer::Deserialize, then serializes the store
 * and checks simple invariants so malformed input that corrupts the counts
 * crashes the fuzzer.
 */

#include "grocery/data/kvstore.hpp"
#include "grocery/data/serializer.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size)
{
    const std::string input(reinterpret_cast<const char*>(data), size);

    // The store is a singleton, so start every run from an empty state.
    auto& store = grocery::data::KVStore::Instance();
    store.clear();

    const grocery::data::Serializer serializer;
    serializer.Deserialize(store, input);

    // Invariant 1: every stored item has a positive count and a non-empty name.
    // Invariant 2: total count never exceeds the number of input lines.
    const auto lineCount = static_cast<long long>(std::count(input.begin(), input.end(), '\n')) + 1;
    long long total = 0;
    for (const auto& [name, count] : store)
    {
        if (name.empty() || count < 1)
        {
            std::abort();
        }
        total += count;
    }
    if (total > lineCount)
    {
        std::abort();
    }

    // Invariant 3: serialized output is empty exactly when the store is empty.
    std::string output;
    serializer.Serialize(store, output);
    if (output.empty() != store.empty())
    {
        std::abort();
    }

    store.clear();
    return 0;
}
