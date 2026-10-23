/**
 * @file controller.cpp
 * @brief Implements delegation between the file facade and stream helpers.
 */

#include "grocery/io/controller.hpp"

#include <iostream>

namespace grocery::io
{
    /**
     * @brief Route file input through the Reader component.
     * @param filename Path of the input file.
     * @param data Output string receiving the file contents.
     */
    void FileController::ReadFromFile(const std::string& filename, std::string& data)
    {
        // Delegate stream handling to the dedicated reader.
        // Keeping this facade thin lets the data layer avoid stream details.
        try
        {
            reader.Read(filename, data);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }

    /**
     * @brief Route file output through the Writer component.
     * @param filename Path of the output file.
     * @param data Serialized text to write.
     */
    void FileController::WriteToFile(const std::string& filename, const std::string& data) const
    {
        // Delegate stream handling to the dedicated writer.
        // Writer owns file-open and error handling for output operations.
        try
        {
            writer.Write(filename, data);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }
} // namespace grocery::io
