/**
 * @file Dialog.cpp
 * @brief Implements item lookup, table, and histogram dialogs.
 */

#include "grocery/ui/dialog.hpp"

#include <cctype>
#include <iostream>
#include <string>

namespace grocery::ui
{
    /**
     * @brief Run the repeated item lookup dialog until the user enters q.
     *
     * Trailing whitespace is removed so an accidental space does not change
     * an otherwise valid item lookup.
     */
    void Dialog::FindItem()
    {
        // replace while true with variable to prevent infinite loops

        std::string itemName;
        while (std::cout << "Enter the name of the item to find (q to return to main menu): " && std::getline(std::cin >> std::ws, itemName))
        {

            // Remove trailing whitespace before comparing or looking up the name.
            // Leading whitespace was already consumed by std::ws above.
            itemName.erase(itemName.find_last_not_of(" \n\r\t") + 1);

            // A leading q is reserved as the dialog's return command.
            if (!itemName.empty() && toupper(itemName[0]) == 'Q')
            {
                std::cout << "Returning to main menu." << std::endl;
                break;
            }

            const std::string result = dataController.FindItem(itemName);
            // Print the already-formatted result without duplicating presentation
            // rules.
            std::cout << result << std::endl;
        }
    }

    /**
     * @brief Display the table view and wait before returning.
     *
     * The pause prevents the next menu redraw from immediately hiding the
     * report from the user.
     */
    void Dialog::ListItemsWithQuantities()
    {
        const std::string result = dataController.ListItemsWithQuantities();
        std::cout << result;

        // Pause so the user can read the report before the menu is redrawn.
        std::cout << "Press Enter to return to the main menu...";
        std::cin.get();
    }

    /**
     * @brief Display the histogram view and wait before returning.
     *
     * The user controls when the menu is redrawn after reviewing the bars.
     */
    void Dialog::PrintHistogram()
    {
        const std::string result = dataController.GetItemsHistogram();
        std::cout << result;

        // Pause for the same reason as the table dialog.
        std::cout << "Press Enter to return to the main menu...";
        std::cin.get();
    }
} // namespace grocery::ui
