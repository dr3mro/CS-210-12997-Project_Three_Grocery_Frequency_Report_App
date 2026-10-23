/**
 * @file reader.cpp
 * @brief Implements line-oriented text-file reading.
 */

#include "grocery/io/reader.hpp"

#include <fstream>
#include <string>

namespace grocery::io
{
    /**
     * @brief Read a file line by line while preserving record boundaries.
     *
     * Each extracted line receives a newline in the destination string so
     * the serializer can process the same record-oriented representation.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Reader::Read(const std::string& filename, std::string& data)
    {
        std::ifstream file(filename);
        if (!file)
        {
            // Report the path so the caller can diagnose missing input files.
            throw std::runtime_error("Could not open file for reading: " + filename);
        }
        std::string line;
        while (std::getline(file, line))
        {
            // Normalize each input record to a newline-separated string.
            data += line + "\n";
        }
        // RAII closes the stream when this local object leaves scope.
    }
} // namespace grocery::io
