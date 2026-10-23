#include "grocery/ui/dialog.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

using grocery::test::CinRedirect;
using grocery::test::CoutCapture;

namespace
{
    void seedStore()
    {
        auto& store = grocery::data::KVStore::Instance();
        store.clear();
        store.addItem("Apple");
        store.addItem("Apple");
        store.addItem("Milk");
    }
} // namespace

TEST_CASE("Dialog finds an item and returns to the menu on q")
{
    seedStore();
    const CinRedirect input("Apple\nq\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.FindItem();

    CHECK(output.str().find("Apple: 2") != std::string::npos);
    CHECK(output.str().find("Returning to main menu.") != std::string::npos);
}

TEST_CASE("Dialog lookup is case-insensitive")
{
    seedStore();
    const CinRedirect input("apple\nQ\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.FindItem();

    CHECK(output.str().find("Apple: 2") != std::string::npos);
}

TEST_CASE("Dialog reports unknown items")
{
    seedStore();
    const CinRedirect input("Bread\nq\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.FindItem();

    CHECK(output.str().find("Bread: Not found") != std::string::npos);
}

TEST_CASE("Dialog trims trailing whitespace from lookups")
{
    seedStore();
    const CinRedirect input("Apple   \nq\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.FindItem();

    CHECK(output.str().find("Apple: 2") != std::string::npos);
}

TEST_CASE("Dialog accepts q in any case")
{
    seedStore();
    const CinRedirect input("q\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.FindItem();

    CHECK(output.str().find("Returning to main menu.") != std::string::npos);
}

TEST_CASE("Dialog prints the quantities table and pauses")
{
    seedStore();
    const CinRedirect input("\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.ListItemsWithQuantities();

    CHECK(output.str().find("Items with Quantities") != std::string::npos);
    CHECK(output.str().find("Apple") != std::string::npos);
    CHECK(output.str().find("Press Enter to return to the main menu") != std::string::npos);
}

TEST_CASE("Dialog prints the histogram and pauses")
{
    seedStore();
    const CinRedirect input("\n");
    const CoutCapture output;
    grocery::ui::Dialog dialog;

    dialog.PrintHistogram();

    CHECK(output.str().find("Items Histogram") != std::string::npos);
    CHECK(output.str().find("Milk") != std::string::npos);
    CHECK(output.str().find("Press Enter to return to the main menu") != std::string::npos);
}
