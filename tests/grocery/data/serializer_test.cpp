#include "grocery/data/kvstore.hpp"
#include "grocery/data/serializer.hpp"
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

TEST_CASE("Serializer writes item-and-count records in order")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    store.addItem("Milk");
    store.addItem("Apple");
    store.addItem("Milk");

    std::string data;
    serializer.Serialize(store, data);

    CHECK(data == "Apple:1\nMilk:2\n");
}

TEST_CASE("Serializer replaces existing output text")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    store.addItem("Eggs");

    std::string data = "stale";
    serializer.Serialize(store, data);

    CHECK(data == "Eggs:1\n");
}

TEST_CASE("Serializer writes nothing for an empty store")
{
    const auto& store = freshStore();
    const grocery::data::Serializer serializer;

    std::string data = "stale";
    serializer.Serialize(store, data);

    CHECK(data.empty());
}

TEST_CASE("Serializer deserializes one item per line")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    serializer.Deserialize(store, "Milk\nBread\nMilk\n");

    CHECK(store.size() == 2);
    CHECK(store.getItem("Milk")->second == 2);
    CHECK(store.getItem("Bread")->second == 1);
}

TEST_CASE("Serializer ignores empty and whitespace-only lines")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    serializer.Deserialize(store, "\n   \n\t\nMilk\n  \r\n");

    CHECK(store.size() == 1);
    CHECK(store.getItem("Milk")->second == 1);
}

TEST_CASE("Serializer strips carriage returns from Windows line endings")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    serializer.Deserialize(store, "Milk\r\nEggs\r\n");

    CHECK(store.getItem("Milk") != store.end());
    CHECK(store.getItem("Eggs") != store.end());
    CHECK(store.getItem("Milk\r") == store.end());
}

TEST_CASE("Serializer increments existing counts when deserializing")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    store.addItem("Milk");
    serializer.Deserialize(store, "milk\nMILK\n");

    CHECK(store.size() == 1);
    CHECK(store.getItem("Milk")->second == 3);
}

TEST_CASE("Serializer preserves text without a trailing newline")
{
    auto& store = freshStore();
    const grocery::data::Serializer serializer;

    serializer.Deserialize(store, "Milk\r\nBread");

    CHECK(store.size() == 2);
    CHECK(store.getItem("Bread")->second == 1);
}
