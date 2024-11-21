#include <vector>
#include <algorithm>
#include <regex>
#include <numeric>
#include <iostream>
#include <map>
#include <string>
#include <any>
#include <tuple>
#include <cassert>

#include "string_utils.h"
#include "nlp.h"
#include "symbols.h"
#include "text_preprocessor.h"

TextPreprocessor::TextPreprocessor(const std::string g2p_dict_file) : g2p(g2p_dict_file) {}

std::vector<int64_t> TextPreprocessor::preprocess(const std::string& text, const std::string& lang, const std::string& text_split_method) {
    std::vector<std::string> texts = pre_seg_text(text, lang, text_split_method);

    std::vector<int64_t> result_phones;

    for (auto& text : texts) {
        auto [phones, norm_text] = segment_and_extract_feature_for_text(text, lang);

        if (phones.empty()) {
            continue;
        }

        // add phones to result_phones
        result_phones.insert(result_phones.end(), phones.begin(), phones.end());
    }

    return result_phones;
}

std::vector<std::string> TextPreprocessor::pre_seg_text(std::string text, const std::string& lang, const std::string& text_split_method) {
    StringUtils::trim_char(text, '\n');
    // StringUtils::replace_all(text, "...", "…");


    // if the first sentence is too short, add a dot in front
    std::string first_sentence = StringUtils::get_first_sentence(text);
    StringUtils::trim(first_sentence);

    std::u32string first_sentence_u32 = StringUtils::converter.from_bytes(first_sentence);
    if (!StringUtils::is_delimiter(first_sentence_u32.front()) && first_sentence_u32.size() < 4) {
        // then, add a dot in front
        text = (lang == "en") ? "." + text : "。" + text;
    }

    if (text_split_method != "cut4") {
        throw std::runtime_error("Only 'cut4' text split method is supported");
    }

    StringUtils::cut4(text);
    StringUtils::replace_all(text, "\n\n", "\n");

    std::vector<std::string> sentences = StringUtils::split(text, '\n');
    sentences = StringUtils::merge_short_sentences(sentences, 5);

    // drop empty sentences
    sentences.erase(std::remove_if(sentences.begin(), sentences.end(), [](const std::string& s) { return StringUtils::is_empty(s); }), sentences.end());

    // add a dot at the end of the sentence if it is missing
    for (auto& sentence : sentences) {
        std::u32string sentence_u32 = StringUtils::converter.from_bytes(sentence);
        if (!StringUtils::is_delimiter(sentence_u32.back())) {
            sentence = sentence + ((lang == "en") ? "." : "。");
        }
    }

    sentences = StringUtils::split_long_sentences(sentences);

    return sentences;
}

std::tuple<std::vector<int>, std::string>
    TextPreprocessor::segment_and_extract_feature_for_text(const std::string& text, const std::string& lang) {
    auto [textlist, langlist] = NLP::seg_text(text, lang);

    if (textlist.empty()) {
        return std::make_tuple(std::vector<int>(), "");
    }

    auto [phones, norm_text] = extract_features(textlist, langlist);

    return std::make_tuple(phones, norm_text);
}

std::tuple<std::vector<int>, std::string>
TextPreprocessor::extract_features(const std::vector<std::string>& textlist, const std::vector<std::string>& langlist) {
    std::vector<int> phones_list;
    std::vector<std::string> norm_text_list;

    for (int i = 0; i < textlist.size(); i++) {
        std::string text = textlist[i];
        std::string lang = langlist[i];

        NLP::normalize_text(text, lang);
        std::vector<std::string> tokens = NLP::tokenize(text.data());
        std::vector<std::string> phones;
        for (auto& token : tokens) {
            std::vector<std::string> new_phones = g2p.translate(token, lang);
            phones.insert(phones.end(), new_phones.begin(), new_phones.end());
        }

        norm_text_list.push_back(text);

        // get index of each phone from NLP::symbols list
        for (const auto& phone : phones) {
            auto it = std::find(Symbols::symbols.begin(), Symbols::symbols.end(), phone);
            if (it != Symbols::symbols.end()) {
                phones_list.push_back(std::distance(Symbols::symbols.begin(), it));
            } else {
                assert(false);
            }
        }
    }

    std::string norm_text = StringUtils::join("", textlist);
    return std::make_tuple(phones_list, norm_text);
}
