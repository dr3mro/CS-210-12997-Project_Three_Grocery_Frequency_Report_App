/**
 * @file writer.cpp
 * @brief Implements text-file output.
 */

#include "grocery/io/writer.hpp"

#include <fstream>

namespace grocery::io
{
    /**
     * @brief Replace the target file with serialized data.
     *
     * The output stream is closed automatically when it leaves this function,
     * ensuring buffered data is flushed before the call returns.
     */
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Writer::Write(const std::string& filename, const std::string& data) const
    {
        std::ofstream file(filename);

        if (!file)
        {
            // Report the path so the caller can diagnose output permissions or paths.
            throw std::runtime_error("Could not open file for writing: " + filename);
        }

        file << data;
        // RAII flushes buffered output and closes the stream at scope exit.
    }
} // namespace grocery::io
