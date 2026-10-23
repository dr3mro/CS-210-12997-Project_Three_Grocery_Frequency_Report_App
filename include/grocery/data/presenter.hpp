#pragma once

/**
 * @file presenter.hpp
 * @brief Public interface for formatting grocery data for display.
 */

#include <map>
#include <string>

namespace grocery::data
{
    constexpr int HISTOGRAM_WIDTH = 38;                                                 // Total column width of the histogram output.
    constexpr int LIST_WIDTH = 38;                                                      // Total column width of the list output.
    constexpr int LIST_NAME_WIDTH = 30;                                                 // Column width reserved for the item name in list output.
    constexpr int HISTOGRAM_NAME_WIDTH = 20;                                            // Column width reserved for the item name in histogram output.
    constexpr int COUNT_WIDTH = 4;                                                      // Column width reserved for the item count in
                                                                                        // both list and histogram output.
    constexpr int BAR_WIDTH = HISTOGRAM_WIDTH - HISTOGRAM_NAME_WIDTH - COUNT_WIDTH - 3; // Remaining width for the bar; -3 accounts for spacing and brackets.
    constexpr const char BAR_FILL = '*';                                                // Character used to fill histogram bars.
    constexpr const char LINE_FILL = ' ';                                               // Character used to pad between name and count in list output.
    constexpr const char SPACE = ' ';                                                   // Character used to pad empty bar space in histogram output.

    /** @brief Produces human-readable views of grocery data. */
    class DataPresenter
    {
    public:
        /** @brief Construct a formatter for grocery data views. */
        DataPresenter() = default;

        /** @brief Destroy the grocery data formatter. */
        virtual ~DataPresenter() = default;

        /**
         * @brief Formats one item and its quantity.
         * @param item Item name.
         * @param quantity Item count.
         * @return Formatted quantity text, or a not-found message for zero.
         */
        // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        std::string FormatItemWithQuantity(const std::string& item, int quantity) const;

        /**
         * @brief Formats item counts as an aligned table.
         * @param items Items and their counts.
         * @return Formatted table text.
         */
        // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        std::string FormatItemsWithQuantities(const std::map<std::string, int>& items) const;

        /**
         * @brief Formats item counts as an asterisk histogram.
         * @param items Items and their counts.
         * @return Formatted histogram text.
         */
        // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        std::string FormatItemsHistogram(const std::map<std::string, int>& items) const;

    }; // class DataPresenter

} // namespace grocery::data
