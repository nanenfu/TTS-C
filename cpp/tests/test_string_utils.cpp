#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "string_utils.h"

TEST_CASE("trim_char should trim char from string", "[text_utils]") {
    std::string str = "\ntest string\n";
    StringUtils::trim_char(str, '\n');
    REQUIRE(str == "test string");

    str = "\n\n\n\ntest string\n\n \n\n\n";
    StringUtils::trim_char(str, '\n');
    REQUIRE(str == "test string\n\n ");
}

TEST_CASE("trim should trim spaces from both sides of the string", "[text_utils]") {
    std::string str = "    test string   ";
    StringUtils::trim(str);
    REQUIRE(str == "test string");

    str = "\t\ttest string\t\t";
    StringUtils::trim(str);
    REQUIRE(str == "test string");
}

TEST_CASE("get_first_sentence should return the first sentence", "[text_utils]") {
    REQUIRE(StringUtils::get_first_sentence("sent1. sent2 sent1. sent3 sent3 sent3.") == "sent1");

    // should work with unicode characters as well:
    REQUIRE(StringUtils::get_first_sentence("sent1。sent2 sent1. sent3 sent3 sent3.") == "sent1");
}

TEST_CASE("split should split the string by the separator", "[text_utils]") {
    std::vector<std::string> result = StringUtils::split("sent1. sent2. sent3.", '.');

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1");
    REQUIRE(result[1] == " sent2");
    REQUIRE(result[2] == " sent3");

    // should work with unicode characters as well:
    result = StringUtils::split("sent1。sent2", U'。');

    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == "sent1");
    REQUIRE(result[1] == "sent2");
}

TEST_CASE("cut4 should split the text into sentences", "[text_utils]") {
    std::string text { "sent1. sent2. sent3." };
    StringUtils::cut4(text);

    REQUIRE(text == "sent1\n sent2\n sent3");
}

TEST_CASE("merge_short_sentences should merge short sentences", "[text_utils]") {
    std::vector<std::string> sentences = {"sent1", "sent2", "sent3", "sent4", "sent5"};
    std::vector<std::string> result = StringUtils::merge_short_sentences(sentences, 6);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1 sent2");
    REQUIRE(result[1] == "sent3 sent4");
    REQUIRE(result[2] == "sent5");
}

TEST_CASE("is_delimiter should return true if the character is a delimiter", "[text_utils]") {
    REQUIRE(StringUtils::is_delimiter('.') == true);
    REQUIRE(StringUtils::is_delimiter(U'。') == true);
    REQUIRE(StringUtils::is_delimiter('a') == false);
}

TEST_CASE("split_long_sentences should split a big text into smaller chunks", "[text_utils]") {
    std::vector<std::string> sentences = {"sent1. sent2. sent3. sent4.", "sent5. sent6."};
    std::vector<std::string> result = StringUtils::split_long_sentences(sentences, 15);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1. sent2.");
    REQUIRE(result[1] == " sent3. sent4.");
    REQUIRE(result[2] == "sent5. sent6.");
}

TEST_CASE("replace_all should replace all occurrences of a substring", "[text_utils]") {
    std::string str = "test, this is a test string";
    StringUtils::replace_all(str, "test", "new");

    REQUIRE(str == "new, this is a new string");
}

TEST_CASE("is_empty should return true if the string is empty", "[text_utils]") {
    REQUIRE(StringUtils::is_empty("") == true);
    REQUIRE(StringUtils::is_empty("   ") == true);
    REQUIRE(StringUtils::is_empty("\t") == true);
    REQUIRE(StringUtils::is_empty("test") == false);
}

TEST_CASE("split_by_delimiters should split the string by delimiters", "[text_utils]") {
    std::vector<std::string> result = StringUtils::split_by_delimiters("sent1. sent2! sent3?");

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1.");
    REQUIRE(result[1] == " sent2!");
    REQUIRE(result[2] == " sent3?");

    // should work with unicode characters as well:
    result = StringUtils::split_by_delimiters("sent1。sent2！sent3？");
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1。");
    REQUIRE(result[1] == "sent2！");
    REQUIRE(result[2] == "sent3？");
}

TEST_CASE("join should join the string with separator", "[text_utils]") {
    std::vector<std::string> strings = {"sent1", "sent2", "sent3"};
    std::string result = StringUtils::join(",", strings);

    REQUIRE(result == "sent1,sent2,sent3");

    // should work with empty separator as well:
    result = StringUtils::join("", strings);
    REQUIRE(result == "sent1sent2sent3");
}
