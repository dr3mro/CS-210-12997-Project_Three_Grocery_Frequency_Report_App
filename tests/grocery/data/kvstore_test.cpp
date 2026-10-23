#include "grocery/data/kvstore.hpp"
#include <doctest/doctest.h>

namespace
{
    grocery::data::KVStore& freshStore()
    {
        auto& store = grocery::data::KVStore::Instance();
        store.clear();
        return store;
    }
} // namespace

TEST_CASE("KVStore::Instance returns the same singleton")
{
    auto& first = grocery::data::KVStore::Instance();
    auto& second = grocery::data::KVStore::Instance();

    CHECK(&first == &second);
}

TEST_CASE("KVStore stores and increments item counts")
{
    auto& store = freshStore();

    store.addItem("Milk");
    CHECK(store.size() == 1);
    CHECK(store.getItem("Milk")->second == 1);

    store.addItem("Milk");
    store.addItem("Milk");
    CHECK(store.size() == 1);
    CHECK(store.getItem("Milk")->second == 3);
}

TEST_CASE("KVStore lookups ignore letter case")
{
    auto& store = freshStore();

    store.addItem("Milk");
    store.addItem("milk");
    store.addItem("MILK");

    CHECK(store.size() == 1);
    CHECK(store.getItem("mILk") != store.end());
    CHECK(store.getItem("mILk")->second == 3);
    CHECK(store.getItem("mILk")->first == "Milk");
}

TEST_CASE("KVStore returns end for missing items")
{
    auto& store = freshStore();

    CHECK(store.getItem("Bread") == store.end());
}

TEST_CASE("KVStore keeps items ordered without regard to case")
{
    auto& store = freshStore();

    store.addItem("banana");
    store.addItem("Apple");
    store.addItem("cherry");

    auto items = store.getItemsWithQuantaties();
    REQUIRE(items.size() == 3);
    CHECK(items.begin()->first == "Apple");
    CHECK(std::next(items.begin(), 1)->first == "banana");
    CHECK(std::next(items.begin(), 2)->first == "cherry");
}

TEST_CASE("KVStore returns a detached copy of the items")
{
    auto& store = freshStore();
    store.addItem("Milk");

    auto items = store.getItemsWithQuantaties();
    items["Milk"] = 99;
    items["Eggs"] = 1;

    CHECK(store.getItem("Milk")->second == 1);
    CHECK(store.getItem("Eggs") == store.end());
}

TEST_CASE("CaseInsensitiveCompare orders mixed-case names")
{
    grocery::data::CaseInsensitiveCompare compare;

    CHECK(compare("apple", "Banana"));
    CHECK_FALSE(compare("Banana", "apple"));
    CHECK_FALSE(compare("Milk", "milk"));
}
