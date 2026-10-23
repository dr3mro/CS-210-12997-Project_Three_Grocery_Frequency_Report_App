#include "grocery/ui/item.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <functional>
#include <stdexcept>
#include <string>

using grocery::test::CoutCapture;

TEST_CASE("Item exposes its display text")
{
    const grocery::ui::Item item("find an item.", [] {}, 1);

    CHECK(item.GetText() == "find an item.");
}

TEST_CASE("Item executes its action")
{
    bool executed = false;
    const grocery::ui::Item item("run", [&executed] { executed = true; }, 1);

    item.ExecuteAction();

    CHECK(executed);
}

TEST_CASE("Item throws when its action is empty")
{
    const grocery::ui::Item item("empty", std::function<void()>{}, 1);

    CHECK_THROWS_AS(item.ExecuteAction(), std::invalid_argument);
}

TEST_CASE("Item prints its number and text")
{
    const grocery::ui::Item item("list items.", [] {}, 1);
    const CoutCapture output;

    item.PrintItem();

    CHECK(output.str().find("to list items.") != std::string::npos);
    CHECK(output.str().find("Press ") == 0);
}

TEST_CASE("Item prints the number it was given")
{
    const grocery::ui::Item item("list items.", [] {}, 3);
    const CoutCapture output;

    item.PrintItem();

    CHECK(output.str() == "Press 3 to list items.\n");
}
