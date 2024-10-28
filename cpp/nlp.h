#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

namespace NLP {
    std::tuple<std::vector<std::string>, std::vector<std::string>> seg_text(const std::string& text, const std::string& lang);
    void normalize_text(std::string& text, const std::string_view lang);
    std::vector<std::string> g2p(const std::string_view text, const std::string_view lang);

    extern std::unordered_map<std::string, std::vector<std::vector<std::string>>> g2p_en_dict;
    void load_g2p_en_dict();
}
