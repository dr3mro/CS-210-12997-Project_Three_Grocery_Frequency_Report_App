#pragma once

/**
 * @file kvstore.hpp
 * @brief Shared in-memory storage for grocery item frequencies.
 */

#include <algorithm>
#include <cctype>
#include <map>
#include <string>

namespace grocery::data
{
    // c++20
    struct CaseInsensitiveCompare
    {
        bool operator()(const std::string& left, const std::string& right) const
        {
            constexpr auto lower = [](unsigned char character)
            {
                return std::tolower(character);
            };
            return std::ranges::lexicographical_compare(left, right, std::ranges::less{}, lower, lower);
        }
    };

    /** @brief Orders item names without considering letter case. */
    // struct CaseInsensitiveCompare
    // {
    //     bool operator()(const std::string& left, const std::string& right) const
    //     {
    //         // Compare the two strings lexicographically, ignoring case.
    //         // instead of using std::tolower on the entire string, we use a lambda to
    //         // compare characters one by one. This avoids creating temporary strings and
    //         // is more efficient.
    //         //
    //         auto lower = [](unsigned char character, unsigned char otherCharacter)
    //         {
    //             return std::tolower(character) < std::tolower(otherCharacter);
    //         };
    //         return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end(), lower);
    //     }
    // };

    /** @brief Singleton map of grocery item names to occurrence counts. */
    class KVStore : public std::map<std::string, int, CaseInsensitiveCompare>
    {
    public:
        /**
         * @brief Returns the process-wide data store.
         * @return Reference to the singleton store.
         */
        static KVStore& Instance();

        /** @brief Prevent copying the process-wide store. */
        KVStore(const KVStore&) = delete;

        /** @brief Prevent assigning to the process-wide store. */
        KVStore& operator=(const KVStore&) = delete;

        /**
         * @brief Increments an item's count, adding it when necessary.
         * @param key Item name to increment.
         */
        void addItem(const std::string& key);

        /**
         * @brief Gets an item's current count.
         * @param key Item name to find.
         * @return Iterator to the item's frequency, or end() when the item is absent.
         */
        auto getItem(const std::string& key) const -> decltype(this->find(key));

        /**
         * @brief Copies all item counts into a standard map.
         * @return Map containing every stored item and its count.
         */
        std::map<std::string, int> getItemsWithQuantaties() const;

    private:
        /** @brief Construct the store; only Instance() may do so. */
        KVStore() = default;

        /** @brief Destroy the process-wide store at program termination. */
        ~KVStore() = default;
    }; // class KVStore
} // namespace grocery::data
