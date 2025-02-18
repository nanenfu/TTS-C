#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "text_preprocessor.h"

std::string lang { "en" };
std::string text_split_method { "cut4" };
std::string text  { "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer sit amet massa mi.\n"
                    "\tDonec ac ipsum non lacus dignissim volutpat... Mauris sollicitudin est lacus,\n"
                    "iaculis pretium dolor finibus at? Donec et vestibulum dolor. Duis ultricies\n\n\n"
                    "\tjusto vel nisi posuere feugiat, suspendisse pharetra arcu quis ante vehicula\n"
                    "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n"
                    "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n"
                    "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n"
                    "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n"
                    "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n"
                    "ac ultrices auctor, tortor odio tempor justo, sed hendrerit sapien neque pretium dui.\n"
                    "\tNunc in enim tellus...                   In eleifend sagittis dui id maximus。\n"
                    "Duis condimentum risus eu urna porta, sed mattis sem molestie."
                    "Sed elit nulla, hendrerit sed quam quis, aliquam pharetra sem.\n\n\n"
                    "Maecenas condimentum libero ac nibh tincidunt, consectetur cursus justo hendrerit.\n"
                    "Phasellus bibendum odio in augue pulvinar, non blandit est ultrices. Duis commodo ve lectus nec dignissim.\n" };

TextPreprocessor text_preprocessor("g2p_en_dict.json");

TEST_CASE("pre_seg_text", "[TextPreprocessor]") {
    std::vector<std::string> result = TextPreprocessor::pre_seg_text(text, lang, text_split_method);

    std::vector<std::string> expected_result {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        " Integer sit amet massa mi.",
        "\tDonec ac ipsum non lacus dignissim volutpat.",
        " Mauris sollicitudin est lacus,",
        "iaculis pretium dolor finibus at? Donec et vestibulum dolor.",
        " Duis ultricies.",
        "\tjusto vel nisi posuere feugiat, suspendisse pharetra arcu quis ante vehicula.",
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit.",
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit.",
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit.",
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit.",
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit.",
        "ac ultrices auctor, tortor odio tempor justo, sed hendrerit sapien neque pretium dui.",
        "\tNunc in enim tellus.",
        "                   In eleifend sagittis dui id maximus。",
        "Duis condimentum risus eu urna porta, sed mattis sem molestie.",
        "Sed elit nulla, hendrerit sed quam quis, aliquam pharetra sem.",
        "Maecenas condimentum libero ac nibh tincidunt, consectetur cursus justo hendrerit.",
        "Phasellus bibendum odio in augue pulvinar, non blandit est ultrices.",
        " Duis commodo ve lectus nec dignissim."
    };

    REQUIRE(result == expected_result);

    text = "Hi. Second sentence.";
    result = text_preprocessor.pre_seg_text(text, lang, text_split_method);
    expected_result = { "Hi  Second sentence." };

    REQUIRE(result == expected_result);
}

TEST_CASE("preprocess", "[TextPreprocessor]") {
    text = "Hello, how are you doing today?";
    std::vector<int64_t> phones = text_preprocessor.preprocess(text, lang, text_split_method);

    // display the result
    std::cout << "phones: ";
    for (auto& phone : phones) {
        std::cout << phone << " ";
    }
    std::cout << std::endl;
}