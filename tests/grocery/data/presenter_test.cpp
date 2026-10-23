#include "grocery/data/presenter.hpp"
#include <doctest/doctest.h>

#include <algorithm>
#include <map>
#include <string>

TEST_CASE("DataPresenter formats a found item")
{
    const grocery::data::DataPresenter presenter;

    CHECK(presenter.FormatItemWithQuantity("Milk", 3) == "Milk: 3");
}

TEST_CASE("DataPresenter reports zero and negative quantities as not found")
{
    const grocery::data::DataPresenter presenter;

    CHECK(presenter.FormatItemWithQuantity("Milk", 0) == "Milk: Not found");
    CHECK(presenter.FormatItemWithQuantity("Milk", -2) == "Milk: Not found");
}

TEST_CASE("DataPresenter formats an ordered quantity table")
{
    const grocery::data::DataPresenter presenter;
    const std::map<std::string, int> items{{"Apple", 2}, {"Milk", 3}};

    const auto table = presenter.FormatItemsWithQuantities(items);

    CHECK(table.find("Items with Quantities") != std::string::npos);
    CHECK(table.find(std::string(grocery::data::LIST_WIDTH, '-')) != std::string::npos);
    CHECK(table.find("Apple") != std::string::npos);
    CHECK(table.find("Milk") != std::string::npos);
    CHECK(table.find('2') != std::string::npos);
    CHECK(table.find('3') != std::string::npos);
    CHECK(table.find("Apple") < table.find("Milk"));
}

TEST_CASE("DataPresenter table has a frame and one line per item")
{
    const grocery::data::DataPresenter presenter;
    const std::map<std::string, int> items{{"Apple", 2}, {"Milk", 3}};

    const auto table = presenter.FormatItemsWithQuantities(items);
    const auto lines = std::count(table.begin(), table.end(), '\n');

    CHECK(lines == 6);
}

TEST_CASE("DataPresenter formats an asterisk histogram")
{
    const grocery::data::DataPresenter presenter;
    const std::map<std::string, int> items{{"Apple", 3}};

    const auto histogram = presenter.FormatItemsHistogram(items);

    CHECK(histogram.find("Items Histogram") != std::string::npos);
    CHECK(histogram.find("Apple") != std::string::npos);
    CHECK(histogram.find("***") != std::string::npos);
    CHECK(histogram.find("[ 3]") != std::string::npos);
    CHECK(histogram.find(std::string(grocery::data::HISTOGRAM_WIDTH, '-')) != std::string::npos);
}

TEST_CASE("DataPresenter histogram scales bars with the item count")
{
    const grocery::data::DataPresenter presenter;
    const std::map<std::string, int> items{{"Apple", 1}, {"Milk", 5}};

    const auto histogram = presenter.FormatItemsHistogram(items);

    CHECK(histogram.find("Apple") < histogram.find("Milk"));
    CHECK(histogram.find("[ 1]") != std::string::npos);
    CHECK(histogram.find("[ 5]") != std::string::npos);
    CHECK(histogram.find("*****") != std::string::npos);
}

TEST_CASE("DataPresenter renders empty collections as an empty framed report")
{
    const grocery::data::DataPresenter presenter;
    const std::map<std::string, int> items;

    const auto table = presenter.FormatItemsWithQuantities(items);
    const auto histogram = presenter.FormatItemsHistogram(items);

    CHECK(table.find("Items with Quantities") != std::string::npos);
    CHECK(histogram.find("Items Histogram") != std::string::npos);
    CHECK(table != "No items found.");
    CHECK(histogram != "No items found.");
}
