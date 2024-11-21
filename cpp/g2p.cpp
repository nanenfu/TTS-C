#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "g2p.h"
#include "string_utils.h"
#include "symbols.h"

/**
 * @brief Load the English grapheme-to-phoneme dictionary.
 */
G2P::G2P(const std::string dict_file) {
    std::ifstream file(dict_file, std::ifstream::in);
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

std::vector<std::string> G2P::translate(std::string token, const std::string lang) {
    // Only English is supported for now
    assert(lang == "en");

    std::vector<std::string> phones;

    if (token.empty()) {
        return phones;
    }

    // check for whitespace
    if (StringUtils::is_empty(token)) {
        return phones;
    }

    // handling special characters
    if (token == "$") {
        phones.push_back("SP2");
        return phones;
    }

    if (token == "^") {
        phones.push_back("SP3");
        return phones;
    }

    // token is in the punctuation list
    if (std::find(Symbols::punctuation.begin(), Symbols::punctuation.end(), token) != Symbols::punctuation.end()) {
        phones.push_back(token);
        return phones;
    }

    // upper case
    std::transform(token.begin(), token.end(), token.begin(), ::toupper);

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

    return phones;
}