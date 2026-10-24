/*
 * Amr Osman
 * amr.osman@snhu.edu
 * CS-210-12997-Project_Three_Grocery_Frequency_Report
 *
 * This program reads grocery item names from an input file, counts each
 * item's frequency, writes the results to a report file, and provides an
 * interactive menu for searching and displaying the collected data.
 */

/**
 * @file main.cpp
 * @brief Program entry point for the Grocery frequency application.
 *
 * This translation unit contains the narrow application boundary. The
 * AppController owns the actual startup sequence, data processing, and
 * interactive user interface.
 *
 * @mainpage Grocery Frequency Report
 *
 * @section overview Overview
 *
 * Grocery Frequency Report is a C++20 console application for counting and
 * reviewing grocery item occurrences. The program reads one item per line from
 * `CS210_Project_Three_Input_File.txt`, stores the counts in memory, writes a
 * report to `frequency.dat`, and then opens an interactive menu.
 *
 * @section features Features
 *
 * - Search for the quantity of an individual item.
 * - List all items and their quantities.
 * - Display an asterisk histogram of item frequencies.
 * - Serialize and deserialize grocery records.
 * - Generate HTML API documentation with Doxygen.
 *
 * @section architecture Architecture
 *
 * The project is organized into four layers:
 *
 * - `grocery::app` coordinates startup and application sequencing.
 * - `grocery::data` stores, serializes, and formats grocery information.
 * - `grocery::io` reads from and writes to files.
 * - `grocery::ui` provides the menu and interactive console dialogs.
 *
 * @section workflow Application workflow
 *
 * 1. Load records from `CS210_Project_Three_Input_File.txt`.
 * 2. Count item occurrences in `grocery::data::KVStore`.
 * 3. Write the serialized report to `frequency.dat`.
 * 4. Create and display the interactive menu.
 * 5. Process searches, tables, histograms, and exit commands.
 *
 * @section building Building and documentation
 *
 * Build the release application with:
 *
 * @code{.sh}
 * make
 * @endcode
 *
 * Generate this documentation with:
 *
 * @code{.sh}
 * make docs
 * @endcode
 *
 * The generated HTML homepage is `docs/html/index.html`.
 */

#include "include/grocery/app/controller.hpp"
#include <grocery/version.hpp>

#include <iostream>
#include <string_view>

/**
 * @brief Starts the Grocery frequency-report application.
 *
 * The controller is created on the stack so its lifetime covers the complete
 * execution of the application. Run() loads the grocery data, creates the
 * output report, and starts the interactive menu.
 *
 * Any exception that escapes the controller is reported here, at the
 * application boundary, and converted into a non-zero process exit status.
 *
 * @return 0 when the application completes normally; 1 when an exception
 *         reaches the entry point.
 */
int main(int argc, char* argv[])
{
    if (argc > 1 && std::string_view(argv[1]) == "--version")
    {
        std::cout << "grocery " << grocery::kVersion << " (" << grocery::kGitHash << ")\n";
        return 0;
    }

    // Keep startup orchestration out of main; the controller owns that workflow.
    try
    {
        // Run the complete load, save, and interactive-menu sequence.
        grocery::app::AppController::Run();

        // A normal return indicates that no fatal application error occurred.
        return 0;
    }
    catch (const std::exception& error)
    {
        // Report the original exception message instead of silently continuing.
        std::cerr << "Application error: " << error.what() << '\n';

        // Non-zero status allows shells and scripts to detect the failure.
        return 1;
    }
}
