#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

namespace NLP {
    std::tuple<std::vector<std::string>, std::vector<std::string>> seg_text(const std::string& text, const std::string& lang);
    void normalize_text(std::string& text, const std::string_view lang);
    std::vector<std::string> tokenize(const std::string& text);
}
