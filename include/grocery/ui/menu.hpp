#pragma once

/**
 * @file menu.hpp
 * @brief Public interface for the interactive menu collection.
 */

#include "grocery/ui/item.hpp"

#include <functional>
#include <vector>
namespace grocery::ui
{
    /** @brief Collection of numbered interactive menu items. */
    class Menu
    {
    private:
        std::vector<Item> items;

    public:
        /** @brief Construct an empty menu. */
        Menu() = default;

        /** @brief Destroy the menu and its owned items. */
        ~Menu() = default;
        /**
         * @brief Adds an item and callback to the menu.
         * @param text Text displayed for the item.
         * @param action Function executed when selected.
         */
        void addItem(const std::string& text, std::function<void()> action);

        /** @brief Prints every menu item. */
        void ShowMenu() const;

        /** @return The number of items currently in the menu. */
        int GetMenuItemCount() const;

        /**
         * @brief Gets an item by zero-based index.
         * @param index Index of the requested item.
         * @return Pointer to the item, or nullptr when the index is invalid.
         */
        Item* getItem(int index);
    }; // class Menu
} // namespace grocery::ui
