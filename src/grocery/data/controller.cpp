/**
 * @file controller.cpp
 * @brief Implements the grocery data-operation facade.
 */

#include "grocery/data/controller.hpp"

namespace grocery::data
{
    /**
     * @brief Return the lazily initialized shared data controller.
     * @return Reference to the one controller used by the application.
     */
    DataController& DataController::Instance()
    {
        static DataController instance;
        return instance;
    }

    /**
     * @brief Read raw grocery records and add them to the shared store.
     * @param filename Path of the input file.
     */
    void DataController::LoadData(const std::string& filename)
    {
        std::string data;
        // Read the whole file first so parsing remains independent of streams.
        fileController.ReadFromFile(filename, data);
        // Deserialization converts each eligible record into a store increment.
        serializer.Deserialize(kvStore, data);
    }

    /**
     * @brief Serialize the shared store and write the resulting report.
     * @param filename Path of the output file.
     */
    void DataController::SaveData(const std::string& filename) const
    {
        std::string data;
        // Serialization is kept separate from file I/O so both responsibilities
        // can be changed independently.
        serializer.Serialize(kvStore, data);
        // Writing happens only after serialization has produced the full buffer.
        fileController.WriteToFile(filename, data);
    }

    /**
     * @brief Look up one item and delegate its display formatting.
     * @param key Item name to search for.
     * @return Formatted quantity or not-found text.
     */
    std::string DataController::FindItem(const std::string& key) const
    {
        const auto item = kvStore.getItem(key);
        std::string result{};
        if (item == kvStore.end())
        {
            // Missing items are represented by a zero count.
            result = presenter.FormatItemWithQuantity(key, 0);
        }
        else
        {
            // The presenter owns user-facing wording and formatting.
            result = presenter.FormatItemWithQuantity(item->first, item->second);
        }
        return result;
    }

    /**
     * @brief Create a formatted table containing all stored item counts.
     * @return Display-ready table text.
     */
    std::string DataController::ListItemsWithQuantities() const
    {
        // Presenters receive copies rather than direct access to the shared store.
        const auto items = kvStore.getItemsWithQuantaties();
        return presenter.FormatItemsWithQuantities(items);
    }

    /**
     * @brief Create a formatted histogram containing all stored item counts.
     * @return Display-ready histogram text.
     */
    std::string DataController::GetItemsHistogram() const
    {
        // Both display modes use the same current frequency snapshot.
        const auto items = kvStore.getItemsWithQuantaties();
        return presenter.FormatItemsHistogram(items);
    }
} // namespace grocery::data
