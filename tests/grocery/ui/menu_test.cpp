#include "grocery/ui/menu.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <string>

using grocery::test::CerrCapture;
using grocery::test::CoutCapture;

TEST_CASE("Menu starts empty")
{
    grocery::ui::Menu menu;

    CHECK(menu.GetMenuItemCount() == 0);
    CHECK(menu.getItem(0) == nullptr);
}

TEST_CASE("Menu adds items and reports its count")
{
    grocery::ui::Menu menu;

    menu.addItem("first", [] {});
    menu.addItem("second", [] {});

    CHECK(menu.GetMenuItemCount() == 2);
}

TEST_CASE("Menu returns items by zero-based index")
{
    grocery::ui::Menu menu;
    menu.addItem("first", [] {});
    menu.addItem("second", [] {});

    auto* item = menu.getItem(1);

    REQUIRE(item != nullptr);
    CHECK(item->GetText() == "second");
}

TEST_CASE("Menu returns nullptr for negative indexes")
{
    grocery::ui::Menu menu;
    menu.addItem("first", [] {});
    CerrCapture errors;

    CHECK(menu.getItem(-1) == nullptr);
    CHECK(errors.str().find("Index out of bounds") != std::string::npos);
}

TEST_CASE("Menu returns nullptr for out-of-range indexes")
{
    grocery::ui::Menu menu;
    menu.addItem("first", [] {});
    CerrCapture errors;

    CHECK(menu.getItem(1) == nullptr);
    CHECK(errors.str().find("Index out of bounds") != std::string::npos);
}

TEST_CASE("Menu prints a heading and its items in order")
{
    grocery::ui::Menu menu;
    menu.addItem("first", [] {});
    menu.addItem("second", [] {});
    CoutCapture output;

    menu.ShowMenu();

    const auto text = output.str();
    CHECK(text.find("Menu:") != std::string::npos);
    CHECK(text.find("first") != std::string::npos);
    CHECK(text.find("second") != std::string::npos);
    CHECK(text.find("first") < text.find("second"));
}

TEST_CASE("Menu items execute their actions")
{
    grocery::ui::Menu menu;
    int calls = 0;
    menu.addItem("count", [&calls] { ++calls; });

    menu.getItem(0)->ExecuteAction();

    CHECK(calls == 1);
}
