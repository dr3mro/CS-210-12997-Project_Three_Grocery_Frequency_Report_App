#include "grocery/app/controller.hpp"
#include "grocery/data/kvstore.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <filesystem>
#include <string>

using grocery::test::CinRedirect;
using grocery::test::CoutCapture;
using grocery::test::CurrentPathGuard;
using grocery::test::readTextFile;
using grocery::test::writeTextFile;

TEST_CASE("Application file names are defined")
{
    CHECK(std::string(INPUT_FILE) == "CS210_Project_Three_Input_File.txt");
    CHECK(std::string(OUTPUT_FILE) == "frequency.dat");
}

TEST_CASE("AppController runs the load, save, and menu workflow")
{
    const auto directory = grocery::test::temporaryPath("app_run");
    std::filesystem::create_directories(directory);
    writeTextFile(directory / INPUT_FILE, "Milk\nBread\nMilk\n");

    grocery::data::KVStore::Instance().clear();
    const CinRedirect input("q\n");

    {
        const CurrentPathGuard guard(directory);
        const CoutCapture output;

        grocery::app::AppController::Run();

        CHECK(output.str().find("Menu:") != std::string::npos);
        CHECK(output.str().find("Exiting the program.") != std::string::npos);
    }

    CHECK(std::filesystem::exists(directory / OUTPUT_FILE));

    const auto report = readTextFile(directory / OUTPUT_FILE);
    CHECK(report.find("Milk:2") != std::string::npos);
    CHECK(report.find("Bread:1") != std::string::npos);

    std::filesystem::remove_all(directory);
}
