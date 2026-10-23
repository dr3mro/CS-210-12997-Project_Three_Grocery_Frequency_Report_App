#pragma once

/**
 * @file controller.hpp
 * @brief Public interface for coordinating grocery data operations.
 */

#include "grocery/data/kvstore.hpp"
#include "grocery/data/presenter.hpp"
#include "grocery/data/serializer.hpp"
#include "grocery/io/controller.hpp"
namespace grocery::data
{
    /**
     * @brief Singleton that coordinates persistence, storage, and presentation.
     *
     * Call Instance() to access the one controller shared by the application.
     */
    class DataController
    {
    public:
        /**
         * @brief Return the process-wide data controller.
         * @return Reference to the singleton controller.
         */
        static DataController& Instance();

        /** @brief Prevent copying the process-wide controller. */
        DataController(const DataController&) = delete;

        /** @brief Prevent assigning the process-wide controller. */
        DataController& operator=(const DataController&) = delete;

        /**
         * @brief Loads item records into the shared singleton data store.
         * @param filename Path of the file to read.
         * @throws std::runtime_error If the file cannot be opened.
         *
         * Loading adds to the existing store; it does not clear records that
         * were loaded previously.
         */
        void LoadData(const std::string& filename);

        /**
         * @brief Serializes the shared data store and writes it to a file.
         * @param filename Path of the file to write.
         * @throws std::runtime_error If the file cannot be opened.
         */
        void SaveData(const std::string& filename) const;

        /**
         * @brief Formats the quantity of one item for display.
         * @param key Item name to look up.
         * @return A quantity message, or a not-found message if absent.
         */
        std::string FindItem(const std::string& key) const;

        /**
         * @brief Formats every stored item and its quantity as a table.
         * @return A display-ready quantity table.
         */
        std::string ListItemsWithQuantities() const;

        /**
         * @brief Formats every stored item as an asterisk histogram.
         * @return A display-ready histogram.
         */
        std::string GetItemsHistogram() const;

    private:
        /** @brief Construct the singleton; only Instance() may call this. */
        DataController() = default;

        /** @brief Destroy the process-wide controller at program termination. */
        ~DataController() = default;

        grocery::io::FileController fileController;
        grocery::data::KVStore& kvStore = grocery::data::KVStore::Instance();
        grocery::data::Serializer serializer;
        grocery::data::DataPresenter presenter;

    }; // class DataController
} // namespace grocery::data
