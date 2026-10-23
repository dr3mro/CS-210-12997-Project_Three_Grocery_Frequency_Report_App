#pragma once

/**
 * @file serializer.hpp
 * @brief Public interface for converting grocery data to and from text.
 */

#include "grocery/data/kvstore.hpp"

#include <string>
namespace grocery::data
{
    /**
     * @brief Converts grocery records to and from the project's text format.
     *
     * The input format is line-oriented: each non-empty line is one grocery
     * item. Serialized output uses the item-and-count format.
     */
    class Serializer
    {
    public:
        /** @brief Construct a serializer. */
        Serializer() = default;

        /** @brief Destroy a serializer. */
        virtual ~Serializer() = default;
        /**
         * @brief Serializes eligible item names and counts into a string.
         * @param kvStore Store whose contents will be serialized.
         * @param data Output string receiving the serialized records.
         *
         * Every stored item is emitted as an item-and-count record.
         */
        void Serialize(const KVStore& kvStore, std::string& data) const;

        /**
         * @brief Deserializes eligible records and adds them to a store.
         * @param kvStore Store that receives the parsed records.
         * @param data Text containing serialized records.
         *
         * Empty and whitespace-only lines are ignored.
         * Existing counts are incremented rather than replaced.
         */
        void Deserialize(KVStore& kvStore, const std::string& data) const;

    private:
        /**
         * @brief Check whether a character is whitespace.
         * @param character Character to inspect.
         * @return true when the character is whitespace.
         */
        static bool IsWhitespace(char character);
    }; // class Serializer
} // namespace grocery::data
