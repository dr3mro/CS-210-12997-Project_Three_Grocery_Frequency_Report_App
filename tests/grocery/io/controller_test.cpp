#include "grocery/io/controller.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <filesystem>

using grocery::test::CinRedirect;
using grocery::test::CoutCapture;
using grocery::test::readTextFile;
using grocery::test::temporaryPath;
using grocery::test::writeTextFile;

TEST_CASE("FileController reads through the reader")
{
    grocery::io::FileController controller;
    const auto file = temporaryPath("file_controller_input.txt");
    writeTextFile(file, "Milk\nBread\n");

    std::string data;
    controller.ReadFromFile(file.string(), data);

    CHECK(data == "Milk\nBread\n");

    std::filesystem::remove(file);
}

TEST_CASE("FileController writes through the writer")
{
    const grocery::io::FileController controller;
    const auto file = temporaryPath("file_controller_output.txt");

    controller.WriteToFile(file.string(), "Milk:1\n");

    CHECK(readTextFile(file) == "Milk:1\n");

    std::filesystem::remove(file);
}

TEST_CASE("FileController reports read failures without throwing")
{
    grocery::io::FileController controller;
    const auto missing = temporaryPath("file_controller_missing.txt");
    const CinRedirect input("\n");
    const CoutCapture output;

    std::string data;
    CHECK_NOTHROW(controller.ReadFromFile(missing.string(), data));
    CHECK(output.str().find("Press Enter to continue") != std::string::npos);
}

TEST_CASE("FileController reports write failures without throwing")
{
    const grocery::io::FileController controller;
    const auto missingDirectory = temporaryPath("file_controller_missing_dir");
    const auto file = missingDirectory / "output.txt";
    const CinRedirect input("\n");
    const CoutCapture output;

    CHECK_NOTHROW(controller.WriteToFile(file.string(), "data"));
    CHECK(output.str().find("Press Enter to continue") != std::string::npos);
}

TEST_CASE("FileController supports polymorphic use")
{
    struct StubController : grocery::io::FileController
    {
        void ReadFromFile(const std::string& /*filename*/, std::string& data) override { data = "stub"; }
    };

    StubController stub;
    grocery::io::FileController& controller = stub;

    std::string data;
    controller.ReadFromFile("ignored", data);

    CHECK(data == "stub");
}
