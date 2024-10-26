#pragma once

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <any>

class TextPreprocessor {
public:
   TextPreprocessor() = default;

   static std::vector<std::map<std::string, std::any>> preprocess(const std::string& text,
                                                            const std::string& lang,
                                                            const std::string& text_split_method);

   static std::vector<std::string> pre_seg_text(std::string text, const std::string& lang,
                                            const std::string& text_split_method);
   static std::tuple<std::vector<unsigned int>, std::string>
      segment_and_extract_feature_for_text(const std::string& text, const std::string& lang);
};
