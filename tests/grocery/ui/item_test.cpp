#include "grocery/ui/item.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <functional>
#include <stdexcept>
#include <string>

using grocery::test::CoutCapture;

TEST_CASE("Item exposes its display text")
{
    const grocery::ui::Item item("find an item.", [] {});

    CHECK(item.GetText() == "find an item.");
}

TEST_CASE("Item executes its action")
{
    bool executed = false;
    const grocery::ui::Item item("run", [&executed] { executed = true; });

    item.ExecuteAction();

    CHECK(executed);
}

TEST_CASE("Item throws when its action is empty")
{
    const grocery::ui::Item item("empty", std::function<void()>{});

    CHECK_THROWS_AS(item.ExecuteAction(), std::invalid_argument);
}

TEST_CASE("Item prints its number and text")
{
    const grocery::ui::Item item("list items.", [] {});
    CoutCapture output;

    item.PrintItem();

    CHECK(output.str().find("to list items.") != std::string::npos);
    CHECK(output.str().find("Press ") == 0);
}

TEST_CASE("Item numbering increases for each created item")
{
    const grocery::ui::Item first("first", [] {});
    std::string firstOutput;
    {
        CoutCapture output;
        first.PrintItem();
        firstOutput = output.str();
    }

    const grocery::ui::Item second("second", [] {});
    CoutCapture output;
    second.PrintItem();

    const auto firstNumber = std::stoi(firstOutput.substr(std::string("Press ").size()));
    const auto secondNumber = std::stoi(output.str().substr(std::string("Press ").size()));

    CHECK(secondNumber == firstNumber + 1);
}
