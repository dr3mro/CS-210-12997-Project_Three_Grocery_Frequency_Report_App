/**
 * @file controller.cpp
 * @brief Implements application startup and workflow coordination.
 */

#include "grocery/app/controller.hpp"

#include "grocery/data/controller.hpp"
#include "grocery/ui/controller.hpp"

namespace grocery::app
{
    /**
     * @brief Load, persist, and interact with grocery frequency data.
     *
     * This method keeps startup sequencing in one place: data must be loaded
     * before the output report or interactive views can be produced.
     */
    void AppController::Run()
    {
        // Use the same data controller that the interactive dialogs access.
        auto& dataController = grocery::data::DataController::Instance();

        // Build the initial frequency table from the supplied input file.
        dataController.LoadData(INPUT_FILE);

        // Persist the generated report before entering the interactive UI.
        dataController.SaveData(OUTPUT_FILE);

        grocery::ui::UIController uiController;
        // Keep the UI controller local so its menu and dialog remain alive
        // for the entire duration of the interactive session.
        // Menu setup is separate from display so the controller can be reused.
        uiController.CreateMenu();
        uiController.ShowMenu();
        uiController.ProcessUserChoice();
    }
} // namespace grocery::app
