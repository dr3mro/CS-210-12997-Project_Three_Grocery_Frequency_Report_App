/**
 * @file serializer.cpp
 * @brief Implements text serialization and deserialization.
 */

#include "grocery/data/serializer.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace grocery::data
{
    /**
     * @brief Determine whether a character is whitespace.
     * @param character Character to inspect.
     * @return true when the character is whitespace; otherwise false.
     */
    bool Serializer::IsWhitespace(char character)
    {
        return std::isspace(static_cast<unsigned char>(character)) != 0;
    }

    /**
     * @brief Convert each stored item into one serialized text record.
     * @param kvStore Store whose entries should be serialized.
     * @param data Output string replaced with the serialized records.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Serializer::Serialize(const KVStore& kvStore, std::string& data) const
    {
        std::ostringstream stream;

        for (const auto& pair : kvStore)
        {
            // Each map entry is already one complete grocery item.
            stream << pair.first << ":" << pair.second << "\n";
        }
        // Replace the caller's output instead of appending to previous data.
        data = stream.str();
    }

    /**
     * @brief Parse input records and add their item names to the store.
     *
     * Empty and whitespace-only lines are ignored. Each remaining line is
     * treated as one complete grocery item.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Serializer::Deserialize(KVStore& kvStore, const std::string& data) const
    {
        std::istringstream stream(data);

        std::string line;

        while (std::getline(stream, line))
        {
            // getline removes '\n' but keeps '\r' from Windows CRLF input.
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            if (!line.empty() && !std::all_of(line.begin(), line.end(), IsWhitespace))
            {
                kvStore.addItem(line);
            }
        }
    }
} // namespace grocery::data
