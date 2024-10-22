#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "text_utils.h"

TEST_CASE("trim_char should trim char from string", "[TextUtils]") {
    std::string str = "\ntest string\n";
    TextUtils::trim_char(str, '\n');
    REQUIRE(str == "test string");

    str = "\n\n\n\ntest string\n\n \n\n\n";
    TextUtils::trim_char(str, '\n');
    REQUIRE(str == "test string\n\n ");
}

TEST_CASE("trim should trim spaces from both sides of the string", "[TextUtils]") {
    std::string str = "    test string   ";
    TextUtils::trim(str);
    REQUIRE(str == "test string");

    str = "\t\ttest string\t\t";
    TextUtils::trim(str);
    REQUIRE(str == "test string");
}

TEST_CASE("get_first_sentence should return the first sentence", "[TextUtils]") {
    REQUIRE(TextUtils::get_first_sentence("sent1. sent2 sent1. sent3 sent3 sent3.") == "sent1");
}

TEST_CASE("split should split the string by the delimiter", "[TextUtils]") {
    std::vector<std::string> result = TextUtils::split("sent1. sent2. sent3.", '.');

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1");
    REQUIRE(result[1] == " sent2");
    REQUIRE(result[2] == " sent3");
}

TEST_CASE("cut4 should split the text into sentences", "[TextUtils]") {
    std::vector<std::string> result = TextUtils::cut4("sent1.   sent2... sent3.");

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1");
    REQUIRE(result[1] == "   sent2");
    REQUIRE(result[2] == " sent3");
}

TEST_CASE("merge_short_sentences should merge short sentences", "[TextUtils]") {
    std::vector<std::string> sentences = {"sent1", "sent2", "sent3", "sent4", "sent5"};
    std::vector<std::string> result = TextUtils::merge_short_sentences(sentences, 6);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1 sent2");
    REQUIRE(result[1] == "sent3 sent4");
    REQUIRE(result[2] == "sent5");
}

TEST_CASE("is_delimiter should return true if the character is in delimiters", "[TextUtils]") {
    REQUIRE(TextUtils::is_delimiter('.') == true);
    REQUIRE(TextUtils::is_delimiter('a') == false);
}

TEST_CASE("split_long_sentences should split a big text into smaller chunks", "[TextUtils]") {
    std::vector<std::string> sentences = {"sent1. sent2.", "sent3. sent4.", "sent5. sent6."};
    std::vector<std::string> result = TextUtils::split_long_sentences(sentences, 15);

    REQUIRE(result.size() > 2);
    REQUIRE(result[0] == "sent1. sent2.");
    REQUIRE(result[1] == "sent3. sent4.");
}

TEST_CASE("replace_all should replace all occurrences of a substring", "[TextUtils]") {
    std::string str = "test, this is a test string";
    TextUtils::replace_all(str, "test", "new");

    REQUIRE(str == "new, this is a new string");
}

TEST_CASE("is_empty should return true if the string is empty", "[TextUtils]") {
    REQUIRE(TextUtils::is_empty("") == true);
    REQUIRE(TextUtils::is_empty("   ") == true);
    REQUIRE(TextUtils::is_empty("\t") == true);
    REQUIRE(TextUtils::is_empty("test") == false);
}

TEST_CASE("split_by_delimiters should split the string by delimiters", "[TextUtils]") {
    std::vector<std::string> result = TextUtils::split_by_delimiters("sent1. sent2! sent3?");

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1.");
    REQUIRE(result[1] == " sent2!");
    REQUIRE(result[2] == " sent3?");
}
