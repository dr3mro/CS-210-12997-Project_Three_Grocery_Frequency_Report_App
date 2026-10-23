#include "grocery/io/writer.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <filesystem>
#include <stdexcept>

using grocery::test::readTextFile;
using grocery::test::temporaryPath;

TEST_CASE("Writer writes a complete text file")
{
    const grocery::io::Writer writer;
    const auto file = temporaryPath("writer_output.txt");

    writer.Write(file.string(), "Milk:2\nBread:1\n");

    CHECK(readTextFile(file) == "Milk:2\nBread:1\n");

    std::filesystem::remove(file);
}

TEST_CASE("Writer replaces the existing file contents")
{
    const grocery::io::Writer writer;
    const auto file = temporaryPath("writer_replace.txt");
    writer.Write(file.string(), "old content");

    writer.Write(file.string(), "new");

    CHECK(readTextFile(file) == "new");

    std::filesystem::remove(file);
}

TEST_CASE("Writer accepts empty data")
{
    const grocery::io::Writer writer;
    const auto file = temporaryPath("writer_empty.txt");
    writer.Write(file.string(), "previous");

    writer.Write(file.string(), "");

    CHECK(readTextFile(file).empty());

    std::filesystem::remove(file);
}

TEST_CASE("Writer throws when a file cannot be opened")
{
    const grocery::io::Writer writer;
    const auto missingDirectory = temporaryPath("writer_missing_dir");
    const auto file = missingDirectory / "output.txt";

    CHECK_THROWS_AS(writer.Write(file.string(), "data"), std::runtime_error);
    CHECK_THROWS_WITH(writer.Write(file.string(), "data"), doctest::Contains("Could not open file for writing"));
}
