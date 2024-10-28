#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <nlohmann/json.hpp>

#include "nlp.h"
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
    std::tuple<std::vector<std::string>, std::vector<std::string>> seg_text(const std::string& text, const std::string& lang) {
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
        std::ifstream file ("g2p_en_dict.json", std::ifstream::in);
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
        return std::vector<std::string>{};
    }
}
