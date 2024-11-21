#include <cassert>
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <algorithm>

#include "nlp.h"
#include "symbols.h"
#include "string_utils.h"

namespace NLP {
/**
 * @brief Segment text into chunks by its language.
 *
 * @param text The text to segment.
 * @param lang The language of the text.
 *
 * @return A tuple containing the segmented text and the language of the text.
 *
 * Currently, this function does nothing and returns the input text and language.
 */
std::tuple<std::vector<std::string>, std::vector<std::string>>
seg_text(const std::string& text, const std::string& lang) {
    return std::make_tuple(std::vector<std::string>{text}, std::vector<std::string>{lang});
}

/**
 * @brief Normalize text.
 *
 * @param text The text to normalize.
 * @param lang The language of the text.
 */
void normalize_text(std::string& text, const std::string_view lang) {
    StringUtils::replace_all(text, ";", ",");
}

std::vector<std::string> tokenize(const std::string& text) {
    std::regex re(R"([,;.\-\?\!\s+])");

    std::sregex_token_iterator iter(text.begin(), text.end(), re, {-1, 0});
    std::sregex_token_iterator end;

    std::vector<std::string> tokens;
    for (; iter != end; ++iter) {
        tokens.push_back(*iter);
    }

    return tokens;
}
}  // Namespace NLP
