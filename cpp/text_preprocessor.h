#pragma once

#include <vector>
#include <string>
#include <map>

class TextPreprocessor {
 public:
    TextPreprocessor() = default;

    static std::vector<std::map<std::string, std::string>> preprocess(const std::string& text,
                                                            const std::string& lang,
                                                            const std::string& text_split_method);

    static std::vector<std::string> pre_seg_text(std::string text, const std::string& lang,
                                            const std::string& text_split_method);
};
