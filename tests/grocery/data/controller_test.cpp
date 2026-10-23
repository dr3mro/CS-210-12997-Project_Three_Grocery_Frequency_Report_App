#include "grocery/data/controller.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <filesystem>

using grocery::test::readTextFile;
using grocery::test::temporaryPath;
using grocery::test::writeTextFile;

namespace
{
    grocery::data::DataController& freshController()
    {
        grocery::data::KVStore::Instance().clear();
        return grocery::data::DataController::Instance();
    }
} // namespace

TEST_CASE("DataController::Instance returns the same singleton")
{
    const auto& first = grocery::data::DataController::Instance();
    const auto& second = grocery::data::DataController::Instance();

    CHECK(&first == &second);
}

TEST_CASE("DataController loads items from a file")
{
    auto& controller = freshController();
    const auto input = temporaryPath("controller_input.txt");
    writeTextFile(input, "Milk\nBread\nMilk\n");

    controller.LoadData(input.string());

    CHECK(grocery::data::KVStore::Instance().getItem("Milk")->second == 2);
    CHECK(grocery::data::KVStore::Instance().getItem("Bread")->second == 1);

    std::filesystem::remove(input);
}

TEST_CASE("DataController adds loaded items to existing data")
{
    auto& controller = freshController();
    const auto first = temporaryPath("controller_first.txt");
    const auto second = temporaryPath("controller_second.txt");
    writeTextFile(first, "Milk\n");
    writeTextFile(second, "Milk\nEggs\n");

    controller.LoadData(first.string());
    controller.LoadData(second.string());

    CHECK(grocery::data::KVStore::Instance().getItem("Milk")->second == 2);
    CHECK(grocery::data::KVStore::Instance().getItem("Eggs")->second == 1);

    std::filesystem::remove(first);
    std::filesystem::remove(second);
}

TEST_CASE("DataController saves the current store to a file")
{
    const auto& controller = freshController();
    const auto output = temporaryPath("controller_output.dat");
    grocery::data::KVStore::Instance().addItem("Milk");
    grocery::data::KVStore::Instance().addItem("Milk");
    grocery::data::KVStore::Instance().addItem("Apple");

    controller.SaveData(output.string());

    CHECK(readTextFile(output) == "Apple:1\nMilk:2\n");

    std::filesystem::remove(output);
}

TEST_CASE("DataController finds stored items")
{
    const auto& controller = freshController();
    grocery::data::KVStore::Instance().addItem("Milk");
    grocery::data::KVStore::Instance().addItem("Milk");

    CHECK(controller.FindItem("Milk") == "Milk: 2");
    CHECK(controller.FindItem("milk") == "Milk: 2");
}

TEST_CASE("DataController reports missing items as not found")
{
    const auto& controller = freshController();

    CHECK(controller.FindItem("Bread") == "Bread: Not found");
}

TEST_CASE("DataController lists items with quantities")
{
    const auto& controller = freshController();
    grocery::data::KVStore::Instance().addItem("Milk");
    grocery::data::KVStore::Instance().addItem("Apple");

    const auto listing = controller.ListItemsWithQuantities();

    CHECK(listing.find("Items with Quantities") != std::string::npos);
    CHECK(listing.find("Apple") != std::string::npos);
    CHECK(listing.find("Milk") != std::string::npos);
}

TEST_CASE("DataController builds a histogram")
{
    const auto& controller = freshController();
    grocery::data::KVStore::Instance().addItem("Milk");
    grocery::data::KVStore::Instance().addItem("Milk");

    const auto histogram = controller.GetItemsHistogram();

    CHECK(histogram.find("Items Histogram") != std::string::npos);
    CHECK(histogram.find("Milk") != std::string::npos);
    CHECK(histogram.find("**") != std::string::npos);
    CHECK(histogram.find("[ 2]") != std::string::npos);
}
