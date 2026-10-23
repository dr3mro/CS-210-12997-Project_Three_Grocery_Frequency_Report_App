/**
 * @file kvstore.cpp
 * @brief Implements the singleton grocery frequency store.
 */

#include "grocery/data/kvstore.hpp"

namespace grocery::data
{
    /**
     * @brief Return the lazily initialized process-wide store.
     * @return Reference to the single shared key-value store.
     */
    KVStore& KVStore::Instance()
    {
        // Function-local initialization creates the store on first use.
        // Every later caller receives the same process-wide reference.
        static KVStore instance;
        return instance;
    }

    /**
     * @brief Increment an item's frequency or create its first entry.
     * @param key Item name whose frequency should increase.
     */
    void KVStore::addItem(const std::string& key)
    {
        // The map comparator makes this lookup case-insensitive while
        // preserving the spelling of the existing map key.
        auto it = find(key);
        if (it != end())
        {
            // Each accepted input record represents one additional occurrence.
            it->second++;
        }
        else
        {
            // New grocery items begin with their first occurrence.
            // std::map keeps keys ordered for the presentation layer.
            this->insert({key, 1});
        }
    }
    /**
     * @brief Look up an item's frequency without modifying the map.
     * @param key Item name to find.
     * @return Iterator to the item's frequency, or end() when the item is absent.
     */
    auto KVStore::getItem(const std::string& key) const -> decltype(this->find(key))
    {
        // The comparator makes this non-mutating lookup case-insensitive.
        auto it = find(key);
        if (it != end())
        {
            // Return the count stored for the matching item.
            return it;
        }

        // Missing items are represented by a zero count.
        return end();
    }

    /**
     * @brief Return a copy of all currently stored item frequencies.
     * @return Snapshot of the store's item-count pairs.
     */
    std::map<std::string, int> KVStore::getItemsWithQuantaties() const
    {
        // Return a copy so callers cannot modify the singleton accidentally.
        // Presenters therefore work from a stable snapshot of the counts.
        return {begin(), end()};
    }

} // namespace grocery::data
