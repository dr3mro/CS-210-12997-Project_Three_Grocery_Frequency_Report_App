/**
 * @file controller.cpp
 * @brief Implements menu construction, rendering, and input dispatch.
 */

#include "grocery/ui/controller.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace grocery::ui
{
    /**
     * @brief Clear the terminal using the standard ANSI escape sequence.
     *
     * The escape sequence clears the visible screen and moves the cursor to
     * row one, column one before the menu is printed.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void UIController::cleanScreen() const
    {
        // ANSI commands clear the screen and move the cursor to row 1, column 1.
        std::cout << "\033[2J\033[1;1H";
    }
    /**
     * @brief Create the four application menu actions.
     *
     * Dialog actions capture the controller to access its Dialog member;
     * the exit action is independent of controller state.
     */
    void UIController::CreateMenu()
    {
        // Each callback captures this controller so it can invoke its dialog.
        menu.addItem("find an item.", [this]()
        {
            // Delegate the lookup loop to the dialog layer.
            dialog.FindItem();
        });
        menu.addItem("list items and quantaties.", [this]()
        {
            // Display the tabular report through the dialog layer.
            dialog.ListItemsWithQuantities();
        });
        menu.addItem("print a histogram.", [this]()
        {
            // Display the frequency bars through the dialog layer.
            dialog.PrintHistogram();
        });

        // Exit does not need controller state, so it uses a non-capturing callback.
        menu.addItem("exit.", []()
        {
            std::cout << "Exiting the program.\n";
            exit(0);
        });
    }

    /** Clear the terminal and render the current menu entries. */
    void UIController::ShowMenu() const
    {
        cleanScreen();
        // Menu owns the entries and is responsible for rendering each one.
        menu.ShowMenu();
    }

    /**
     * @brief Process menu input until the user selects a quit command.
     *
     * Input is parsed as a complete token so values such as "2abc" cannot
     * accidentally be accepted as menu choice two.
     */
    void UIController::ProcessUserChoice()
    {
        std::string input;
        bool keepRunning = true;
        do
        {
            std::cout << "Please select menu option (1-" << menu.GetMenuItemCount() << ", q to quit): ";

            if (!std::getline(std::cin, input))
            {
                keepRunning = false;
                break;
            }

            // A letter command bypasses numeric parsing and requests exit.
            if (input == "q" || input == "Q")
            {
                std::cout << "Exiting the program.\n";
                keepRunning = false;
                break;
            }

            std::istringstream inputStream(input);
            int choice;
            char extra;

            // Reject empty input, trailing characters, and out-of-range numbers.
            if (!(inputStream >> choice) || (inputStream >> extra) || choice < 1 || choice > menu.GetMenuItemCount())
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
                continue;
            }
            // Convert the user's one-based choice into the menu's zero-based index.

            auto* item = menu.getItem(choice - 1);
            if (item != nullptr)
            {
                try
                {
                    item->ExecuteAction();
                }
                catch (const std::exception& error)
                {
                    std::cerr << "Error executing menu action: " << error.what() << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                }
                // Redraw after an action so the next prompt starts at a clean menu.
                ShowMenu();
            }
            else
            {
                std::cerr << "Error: Menu item not found for choice " << choice << std::endl;
            }
        } while (keepRunning);
    }
} // namespace grocery::ui
