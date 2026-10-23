/**
 * @file presenter.cpp
 * @brief Implements table, histogram, and lookup formatting.
 */

#include "grocery/data/presenter.hpp"

#include <iomanip>
#include <sstream>

namespace grocery::data
{
    /**
     * @brief Render one item's lookup result.
     * @param item Item name being displayed.
     * @param quantity Frequency associated with the item.
     * @return User-facing quantity or not-found message.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    std::string DataPresenter::FormatItemWithQuantity(const std::string& item, int quantity) const
    {
        if (quantity > 0)
        {
            // Positive quantities are shown directly to the user.
            return item + ": " + std::to_string(quantity);
        }

        // A zero count indicates that the requested item is absent.
        return item + ": Not found";
    }

    /**
     * @brief Render all item counts in a fixed-width table.
     * @param items Item names and their frequencies.
     * @return Complete table formatted for console output.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    std::string DataPresenter::FormatItemsWithQuantities(const std::map<std::string, int>& items) const
    {
        std::stringstream result;

        // The three header lines establish the table's visual boundary.
        result << std::string(LIST_WIDTH, '-') << std::endl;
        result << std::setw(LIST_WIDTH) << std::left << "Items with Quantities" << std::endl;
        result << std::string(LIST_WIDTH, '-') << std::endl;

        result << std::setfill(LINE_FILL);

        // std::map iteration keeps the report in alphabetical item order.
        for (const auto& item : items)
        {
            // Reserve 25 columns for names and four right-aligned columns for counts.
            result << std::setw(LIST_NAME_WIDTH) << std::left << item.first << std::setw(COUNT_WIDTH) << std::right << std::to_string(item.second) << std::endl;
        }

        // Close the table after the final record.
        result << std::string(LIST_WIDTH, '-') << std::endl;
        // Keep the fallback for future callers that may pass an empty stream.
        return result.str().empty() ? "No items found." : result.str();
    }

    /**
     * @brief Render all item counts as an asterisk histogram.
     * @param items Item names and their frequencies.
     * @return Complete histogram formatted for console output.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    std::string DataPresenter::FormatItemsHistogram(const std::map<std::string, int>& items) const
    {
        std::stringstream result;

        // The histogram uses a wider frame to leave room for the bars.
        result << std::string(HISTOGRAM_WIDTH, '-') << std::endl;
        result << std::setw(HISTOGRAM_WIDTH) << std::left << "Items Histogram" << std::endl;
        result << std::string(HISTOGRAM_WIDTH, '-') << std::endl;

        // set empty fill character for the histogram bars
        result << std::setfill(LINE_FILL);

        for (const auto& item : items)
        {
            // Align the item name, bar, and numeric count for easy comparison.
            result << std::setw(HISTOGRAM_NAME_WIDTH) << std::left << item.first << std::setw(COUNT_WIDTH) << std::right << std::setw(BAR_WIDTH) << std::right
                   << std::string(static_cast<std::size_t>(item.second), BAR_FILL) << SPACE << std::left << "[" << std::setw(2) << std::right << item.second
                   << "] ";
            result << std::endl;
        }

        // Close the histogram after all bars have been written.
        result << std::string(HISTOGRAM_WIDTH, '-') << std::endl;
        // Return the complete assembled view as one printable string.
        return result.str().empty() ? "No items found." : result.str();
    }

} // namespace grocery::data
