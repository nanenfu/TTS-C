#pragma once

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <any>

class TextPreprocessor {
public:
    TextPreprocessor() = default;

    static std::vector<int64_t> preprocess(const std::string& text, const std::string& lang,
                                           const std::string& text_split_method);

    static std::vector<std::string> pre_seg_text(std::string text, const std::string& lang,
                                                const std::string& text_split_method);
    static std::tuple<std::vector<int>, std::string>
        segment_and_extract_feature_for_text(const std::string& text, const std::string& lang);

    static std::tuple<std::vector<int>, std::string>
        extract_features(const std::vector<std::string>& textlist, const std::vector<std::string>& langlist);
};
