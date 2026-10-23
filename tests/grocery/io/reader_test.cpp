#include "grocery/io/reader.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <filesystem>
#include <stdexcept>

using grocery::test::temporaryPath;
using grocery::test::writeTextFile;

TEST_CASE("Reader reads a complete file line by line")
{
    grocery::io::Reader reader;
    auto file = temporaryPath("reader_input.txt");
    writeTextFile(file, "Milk\nBread\nEggs\n");

    std::string data;
    reader.Read(file.string(), data);

    CHECK(data == "Milk\nBread\nEggs\n");

    std::filesystem::remove(file);
}

TEST_CASE("Reader appends to the destination string")
{
    grocery::io::Reader reader;
    auto file = temporaryPath("reader_append.txt");
    writeTextFile(file, "Milk\n");

    std::string data = "existing:";
    reader.Read(file.string(), data);

    CHECK(data == "existing:Milk\n");

    std::filesystem::remove(file);
}

TEST_CASE("Reader leaves the destination unchanged for an empty file")
{
    grocery::io::Reader reader;
    auto file = temporaryPath("reader_empty.txt");
    writeTextFile(file, "");

    std::string data = "existing";
    reader.Read(file.string(), data);

    CHECK(data == "existing");

    std::filesystem::remove(file);
}

TEST_CASE("Reader preserves blank lines")
{
    grocery::io::Reader reader;
    auto file = temporaryPath("reader_blank.txt");
    writeTextFile(file, "Milk\n\nBread\n");

    std::string data;
    reader.Read(file.string(), data);

    CHECK(data == "Milk\n\nBread\n");

    std::filesystem::remove(file);
}

TEST_CASE("Reader throws when a file cannot be opened")
{
    grocery::io::Reader reader;
    auto missing = temporaryPath("reader_missing.txt");

    std::string data;
    CHECK_THROWS_AS(reader.Read(missing.string(), data), std::runtime_error);
    CHECK_THROWS_WITH(reader.Read(missing.string(), data), doctest::Contains("Could not open file for reading"));
}
