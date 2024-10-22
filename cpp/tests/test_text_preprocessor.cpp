#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "text_preprocessor.h"

TEST_CASE("pre_seg_text should add dot in front when first seq is small and first chat is not in split", "[TextPreprocessor]") {
    std::string lang = "en";
    std::string text_split_method = "cut4";
    std::string text = "Hi! the two of us. we can make it if we try\n";

    std::vector<std::string> result = TextPreprocessor::pre_seg_text(text, lang, text_split_method);

    // display the result
    for (const auto& txt : result) {
        std::cout << txt << std::endl;
    }

    REQUIRE(result.front() == "just the two of us\n");
}
