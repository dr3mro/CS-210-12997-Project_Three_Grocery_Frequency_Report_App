#include "grocery/ui/controller.hpp"
#include "support/test_support.hpp"
#include <doctest/doctest.h>

#include <string>

using grocery::test::CinRedirect;
using grocery::test::CoutCapture;

namespace
{
    std::size_t countOccurrences(const std::string& text, const std::string& needle)
    {
        std::size_t count = 0;
        std::size_t position = 0;
        while ((position = text.find(needle, position)) != std::string::npos)
        {
            ++count;
            position += needle.size();
        }
        return count;
    }
} // namespace

TEST_CASE("UIController clears the screen with an ANSI sequence")
{
    const grocery::ui::UIController controller;
    const CoutCapture output;

    controller.cleanScreen();

    CHECK(output.str() == "\033[2J\033[1;1H");
}

TEST_CASE("UIController creates the four supported menu entries")
{
    grocery::ui::UIController controller;
    const CoutCapture output;

    controller.CreateMenu();
    controller.ShowMenu();

    const auto text = output.str();
    CHECK(text.find("Menu:") != std::string::npos);
    CHECK(text.find("find an item.") != std::string::npos);
    CHECK(text.find("list items and quantaties.") != std::string::npos);
    CHECK(text.find("print a histogram.") != std::string::npos);
    CHECK(text.find("exit.") != std::string::npos);
    CHECK(text.find("Press 4 to exit.") != std::string::npos);
}

TEST_CASE("UIController leaves the menu on q")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("q\n");
    const CoutCapture output;

    controller.ProcessUserChoice();

    CHECK(output.str().find("Please select menu option") != std::string::npos);
    CHECK(output.str().find("Exiting the program.") != std::string::npos);
}

TEST_CASE("UIController accepts an uppercase quit command")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("Q\n");
    const CoutCapture output;

    controller.ProcessUserChoice();

    CHECK(output.str().find("Exiting the program.") != std::string::npos);
}

TEST_CASE("UIController rejects empty and non-numeric choices")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("\nabc\nq\n");
    const CoutCapture output;

    controller.ProcessUserChoice();

    CHECK(countOccurrences(output.str(), "Invalid choice. Please try again.") == 2);
}

TEST_CASE("UIController rejects choices with trailing characters")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("2abc\nq\n");
    const CoutCapture output;

    controller.ProcessUserChoice();

    CHECK(countOccurrences(output.str(), "Invalid choice. Please try again.") == 1);
}

TEST_CASE("UIController rejects out-of-range choices")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("0\n5\nq\n");
    const CoutCapture output;

    controller.ProcessUserChoice();

    CHECK(countOccurrences(output.str(), "Invalid choice. Please try again.") == 2);
}

TEST_CASE("UIController stops cleanly when input ends")
{
    grocery::ui::UIController controller;
    controller.CreateMenu();
    const CinRedirect input("");
    const CoutCapture output;

    CHECK_NOTHROW(controller.ProcessUserChoice());
    CHECK(output.str().find("Exiting the program.") == std::string::npos);
}
