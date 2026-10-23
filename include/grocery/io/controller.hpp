#pragma once

/**
 * @file controller.hpp
 * @brief Public file-I/O facade used by the data layer.
 */

#include <grocery/io/reader.hpp>
#include <grocery/io/writer.hpp>

#include <string>

namespace grocery::io
{
    /** @brief Facade for the application's file reading and writing operations. */
    class FileController
    {
    public:
        /** @brief Construct a file-operation facade. */
        FileController() = default;

        /** @brief Destroy the file-operation facade. */
        virtual ~FileController() = default;
        /**
         * @brief Reads a complete file into a string.
         * @param filename Path of the file to read.
         * @param data Output string receiving the file contents.
         * @throws std::runtime_error If the file cannot be opened.
         */
        virtual void ReadFromFile(const std::string& filename, std::string& data);

        /**
         * @brief Writes a string to a file.
         * @param filename Path of the file to write.
         * @param data Text to write.
         * @throws std::runtime_error If the file cannot be opened.
         */
        virtual void WriteToFile(const std::string& filename, const std::string& data) const;

    private:
        Reader reader;
        Writer writer;
    }; // class FileController
} // namespace grocery::io