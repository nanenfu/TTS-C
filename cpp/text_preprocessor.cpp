#include <vector>
#include <algorithm>
#include <regex>
#include <numeric>
#include <iostream>

#include "text_utils.h"
#include "text_preprocessor.h"

std::vector<std::map<std::string, std::string>> TextPreprocessor::preprocess(const std::string& text, const std::string& lang, const std::string& text_split_method) {
    std::vector<std::string> texts = pre_seg_text(text, lang, text_split_method);

    std::vector<std::map<std::string, std::string>> result;
    return result;
}

std::vector<std::string> TextPreprocessor::pre_seg_text(std::string text, const std::string& lang, const std::string& text_split_method) {
    text = TextUtils::trim_char(text, '\n');

    std::string first_sentence = TextUtils::get_first_sentence(text);
    first_sentence = TextUtils::trim(first_sentence);

    if (TextUtils::is_split(text.front()) && first_sentence.size() < 4) {
        // then, add a dot in front
        text = (lang == "en") ? "." + text : "。" + text;
    }

    if (text_split_method != "cut4") {
        throw std::runtime_error("Only 'cut4' text split method is supported");
    }

    std::vector<std::string> sentences = TextUtils::cut4(text);
    sentences = TextUtils::merge_short_sentences(sentences, 5);

    // drop empty sentences
    sentences.erase(std::remove_if(sentences.begin(), sentences.end(), [](const std::string& s) { return TextUtils::trim(s).empty(); }), sentences.end());

    // add a dot at the end of the sentence if it is missing
    for (auto& sentence : sentences) {
        if (!TextUtils::is_split(sentence.back())) {
            std::string terminal = (lang == "en") ? "." : "。";
            sentence = sentence + terminal;
        }
    }

    return sentences;
}
