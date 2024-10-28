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

TEST_CASE("load_g2p_en_dict should load g2p_en_dict", "[NLP]") {
    NLP::load_g2p_en_dict();

    REQUIRE(NLP::g2p_en_dict["ZZZZ"][0][0] == "Z");
}
