/**
 * @file item.cpp
 * @brief Implements menu-item callbacks and display behavior.
 */

#include "grocery/ui/item.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace grocery::ui
{
    /**
     * @brief Store the text and callback for one menu entry.
     *
     * The constructor receives both values by value so callers may provide
     * either lvalues or temporaries. Moving them into the object avoids an
     * unnecessary second allocation or copy.
     */
    Item::Item(std::string label, std::function<void()> callback, int number)
        // Move the supplied values into their long-lived member variables.
        // The owning menu supplies the number, so every menu starts at 1.
        : text(std::move(label)), action(std::move(callback)), index(number)
    {
    }

    /**
     * @brief Return the label without copying it.
     *
     * The reference remains valid while this Item object is alive and its
     * text member is not modified.
     */
    const std::string& Item::GetText() const
    {
        // Expose the stored label without transferring ownership or copying it.
        return text;
    }

    /**
     * @brief Invoke the action associated with the menu entry.
     *
     * Menu construction supplies the callback; this class deliberately does
     * not need to know whether the action opens a dialog or exits the program.
     */
    void Item::ExecuteAction() const
    {
        // The callback contains the behavior associated with this menu entry.
        // std::function safely stores either a free function or a captured lambda.
        if (!action)
        {
            throw std::invalid_argument("Menu item requires a valid action");
            return;
        }
        action();
    }

    /**
     * @brief Write the user-facing representation of this menu entry.
     *
     * The index is stored on the item so its displayed number remains paired
     * with its label when the menu is rendered.
     */
    void Item::PrintItem() const
    {
        // Keep the displayed number and label on one line for menu selection.
        std::cout << "Press " << index << " to " << text << std::endl;
    }
} // namespace grocery::ui
