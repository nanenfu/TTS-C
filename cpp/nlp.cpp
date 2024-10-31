#include <cassert>
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "nlp.h"
#include "symbols.h"
#include "string_utils.h"

namespace NLP {
// unordered_map for faster lookup
std::unordered_map<std::string, std::vector<std::vector<std::string>>> g2p_en_dict;

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

/**
 * @brief Load the English grapheme-to-phoneme dictionary.
 */
void load_g2p_en_dict() {
    std::ifstream file("g2p_en_dict.json", std::ifstream::in);
    nlohmann::json j;
    file >> j;

    for (auto& [key, value] : j.items()) {
        std::vector<std::vector<std::string>> lists;
        for (auto& list : value) {
            lists.push_back(list.get<std::vector<std::string>>());
        }
        g2p_en_dict[key] = lists;
    }
    assert(g2p_en_dict.empty() == false);
}

std::vector<std::string> g2p(const std::string_view text, const std::string_view lang) {
    // Only English is supported for now
    assert(lang == "en");

    // Load the dictionary on the first run
    if (g2p_en_dict.empty()) {
        load_g2p_en_dict();
    }
    std::vector<std::string> tokens = tokenize(text.data());
    std::vector<std::string> phones;

    for (auto& token : tokens) {
        // upper case
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);

        if (token.empty()) {
            continue;
        }

        // check for whitespace
        if (StringUtils::is_empty(token)) {
            continue;
        }

        // handling special characters
        if (token == "$") {
            phones.push_back("SP2");
            continue;
        }

        if (token == "^") {
            phones.push_back("SP3");
            continue;
        }

        // token is in the punctuation list
        if (std::find(punctuation.begin(), punctuation.end(), token) != punctuation.end()) {
            phones.push_back(token);
            continue;
        }

        if (g2p_en_dict.find(token) != g2p_en_dict.end()) {
            // copy to phones
            for (const auto& phns : g2p_en_dict[token]) {
                for (const auto& phone : phns) {
                    phones.push_back(phone);
                }
            }
        } else {
            std::cerr << "No phonetic transcription for " << token << std::endl;
        }
    }

    return phones;
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
