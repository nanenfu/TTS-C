#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "text_utils.h"

TEST_CASE("trim_char should trim char from string\n", "[TextUtils]") {
    REQUIRE(TextUtils::trim_char("\n\n\n\ntest string\n\n\n\n\n", '\n') == "test string");
    REQUIRE(TextUtils::trim_char("\n\n\n\ntest string\n\n \n\n\n", '\n') == "test string\n\n ");
}

TEST_CASE("trim should trim spaces from both sides of the string\n", "[TextUtils]") {
    REQUIRE(TextUtils::trim("    test string   ") == "test string");
    REQUIRE(TextUtils::trim("\t\ttest string\t\t") == "test string");
}

TEST_CASE("get_first_sentence should return the first sentence\n", "[TextUtils]") {
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
    REQUIRE(result[1] == "sent2");
    REQUIRE(result[2] == "sent3");
}

TEST_CASE("merge_short_sentences should merge short sentences", "[TextUtils]") {
    std::vector<std::string> sentences = {"sent1", "sent2", "sent3", "sent4", "sent5"};
    std::vector<std::string> result = TextUtils::merge_short_sentences(sentences, 6);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "sent1 sent2");
    REQUIRE(result[1] == "sent3 sent4");
    REQUIRE(result[2] == "sent5");
}

TEST_CASE("is_split should return true if the character is in splits", "[TextUtils]") {
    REQUIRE(TextUtils::is_split('.') == true);
    REQUIRE(TextUtils::is_split('a') == false);

    std::string str = ".";
    REQUIRE(TextUtils::is_split(str) == true);

    str = "a";
    REQUIRE(TextUtils::is_split(str) == false);
}