#pragma once

#include <tuple>
#include <string>
#include <vector>

namespace nlp {
    std::tuple<std::vector<std::string>, std::vector<std::string>> seg_text(const std::string& text, const std::string& lang);
}
