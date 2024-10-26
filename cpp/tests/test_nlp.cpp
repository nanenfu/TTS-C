#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "nlp.h"

TEST_CASE("seg_text", "[NLP]") {
    auto [textlist, langlist] = nlp::seg_text("Hello, world!", "en");

    REQUIRE(textlist.size() == 1);
    REQUIRE(textlist[0] == "Hello, world!");

    REQUIRE(langlist.size() == 1);
    REQUIRE(langlist[0] == "en");
}
