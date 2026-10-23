#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace grocery::test
{
    class CoutCapture
    {
    public:
        CoutCapture() : previous(std::cout.rdbuf(stream.rdbuf())) {}

        ~CoutCapture() { std::cout.rdbuf(previous); }

        std::string str() const { return stream.str(); }

    private:
        std::ostringstream stream;
        std::streambuf* previous;
    };

    class CerrCapture
    {
    public:
        CerrCapture() : previous(std::cerr.rdbuf(stream.rdbuf())) {}

        ~CerrCapture() { std::cerr.rdbuf(previous); }

        std::string str() const { return stream.str(); }

    private:
        std::ostringstream stream;
        std::streambuf* previous;
    };

    class CinRedirect
    {
    public:
        explicit CinRedirect(std::string input) : stream(std::move(input)), previous(std::cin.rdbuf(stream.rdbuf())) { std::cin.clear(); }

        ~CinRedirect()
        {
            std::cin.rdbuf(previous);
            std::cin.clear();
        }

    private:
        std::istringstream stream;
        std::streambuf* previous;
    };

    class CurrentPathGuard
    {
    public:
        explicit CurrentPathGuard(const std::filesystem::path& path) : previous(std::filesystem::current_path()) { std::filesystem::current_path(path); }

        ~CurrentPathGuard() { std::filesystem::current_path(previous); }

    private:
        std::filesystem::path previous;
    };

    inline std::filesystem::path temporaryPath(const std::string& name)
    {
        static int counter = 0;
        ++counter;
        return std::filesystem::temp_directory_path() / ("grocery_test_" + std::to_string(counter) + "_" + name);
    }

    inline void writeTextFile(const std::filesystem::path& path, const std::string& content)
    {
        std::ofstream file(path);
        file << content;
    }

    inline std::string readTextFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
} // namespace grocery::test
