#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "g2p.h"

G2P g2p("g2p_en_dict.json");

TEST_CASE("g2p should split sentence into phone", "[NLP]") {
    std::vector<std::string> phones = g2p.translate("D", "en");

    REQUIRE(phones[0] == "D");
    REQUIRE(phones.size() == 2);
}
