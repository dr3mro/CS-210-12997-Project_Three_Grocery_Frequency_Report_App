#pragma once

/**
 * @file item.hpp
 * @brief Public interface for an individual menu item.
 */

#include <functional>
#include <string>

namespace grocery::ui
{
    /** @brief A numbered menu entry and its associated callback. */
    class Item
    {
    public:
        /**
         * @brief Creates a menu item.
         * @param label Text displayed for the item.
         * @param callback Function executed when the item is selected.
         * @param number One-based number shown to the user for this item.
         *
         * An empty callback is permitted by the type system, but invoking an
         * empty callback later may throw std::bad_function_call.
         */
        Item(std::string label, std::function<void()> callback, int number);

        /** @brief Destroy the menu item and its callback. */
        ~Item() = default;

        /** @return The item's display text. */
        const std::string& GetText() const;

        /**
         * @brief Executes the item's callback.
         * @throws std::bad_function_call If the callback is empty.
         */
        void ExecuteAction() const;

        /** @brief Prints the item's number and display text. */
        void PrintItem() const;

    private:
        std::string text;
        std::function<void()> action;
        int index{0};
    }; // class Item

} // namespace grocery::ui
