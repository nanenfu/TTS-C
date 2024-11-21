#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "nlp.h"

TEST_CASE("seg_text", "[NLP]") {
    auto [textlist, langlist] = NLP::seg_text("Hello, world!", "en");

    REQUIRE(textlist.size() == 1);
    REQUIRE(textlist[0] == "Hello, world!");

    REQUIRE(langlist.size() == 1);
    REQUIRE(langlist[0] == "en");
}

TEST_CASE("normalize_text", "[NLP]") {
    std::string text = "Hello; world!";
    NLP::normalize_text(text, "en");

    REQUIRE(text == "Hello, world!");
}

TEST_CASE("tokenize should split sentence into tokens", "[NLP]") {
    std::vector<std::string> tokens = NLP::tokenize("Lorem? ipsum! dolor sit amet.");

    REQUIRE(tokens.size() == 14);
    REQUIRE(tokens[0] == "Lorem");
}
