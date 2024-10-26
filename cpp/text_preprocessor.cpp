#include <vector>
#include <algorithm>
#include <regex>
#include <numeric>
#include <iostream>
#include <map>
#include <string>
#include <any>
#include <tuple>

#include "text_utils.h"
#include "text_preprocessor.h"

std::vector<std::map<std::string, std::any>> TextPreprocessor::preprocess(const std::string& text, const std::string& lang, const std::string& text_split_method) {
    std::vector<std::string> texts = pre_seg_text(text, lang, text_split_method);

    std::vector<std::map<std::string, std::any>> result;

    for (auto& text : texts) {
        auto [phones, norm_text] = segment_and_extract_feature_for_text(text, lang);

        if (phones.empty()) {
            continue;
        }

        std::map<std::string, std::any> text_map;
        text_map["norm_text"] = norm_text;
        text_map["phones"] = phones;

        result.push_back(text_map);
    }

    return result;
}

std::vector<std::string> TextPreprocessor::pre_seg_text(std::string text, const std::string& lang, const std::string& text_split_method) {
    text_utils::trim_char(text, '\n');
    // text_utils::replace_all(text, "...", "…");


    // if the first sentence is too short, add a dot in front
    std::string first_sentence = text_utils::get_first_sentence(text);
    text_utils::trim(first_sentence);

    std::u32string first_sentence_u32 = text_utils::converter.from_bytes(first_sentence);
    if (!text_utils::is_delimiter(first_sentence_u32.front()) && first_sentence_u32.size() < 4) {
        // then, add a dot in front
        text = (lang == "en") ? "." + text : "。" + text;
    }

    if (text_split_method != "cut4") {
        throw std::runtime_error("Only 'cut4' text split method is supported");
    }

    text_utils::cut4(text);
    text_utils::replace_all(text, "\n\n", "\n");

    std::vector<std::string> sentences = text_utils::split(text, '\n');
    sentences = text_utils::merge_short_sentences(sentences, 5);

    // drop empty sentences
    sentences.erase(std::remove_if(sentences.begin(), sentences.end(), [](const std::string& s) { return text_utils::is_empty(s); }), sentences.end());

    // add a dot at the end of the sentence if it is missing
    for (auto& sentence : sentences) {
        std::u32string sentence_u32 = text_utils::converter.from_bytes(sentence);
        if (!text_utils::is_delimiter(sentence_u32.back())) {
            sentence = sentence + ((lang == "en") ? "." : "。");
        }
    }

    sentences = text_utils::split_long_sentences(sentences);

    return sentences;
}

std::tuple<std::vector<unsigned int>, std::string>
TextPreprocessor::segment_and_extract_feature_for_text(const std::string& text, const std::string& lang) {
    


    std::vector<unsigned int> phones;
    std::string norm_text;

    return std::make_tuple(phones, norm_text);
}
