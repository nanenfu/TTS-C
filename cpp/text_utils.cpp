#include <numeric>
#include <regex>
#include <sstream>
#include <iostream>
#include <string>

#include "text_utils.h"

const std::vector<std::string> TextUtils::splits = {"，", "。", "？", "！", ",", ".", "?", "!", "~", ":", "：", "—", "…"};

std::string TextUtils::trim_char(const std::string& str, char c) {
    auto start = str.begin();
    auto end = str.end();

    // Trim leading spaces
    while (start != end && *start == c) {
        start++;
    }

    // Trim trailing spaces
    while (end != start && *(end - 1) == c) {
        end--;
    }

    return std::string(start, end);  // Return the trimmed string
}

// Function to trim leading and trailing whitespace
std::string TextUtils::trim(const std::string& str) {
    auto start = str.begin();
    auto end = str.end();

    // Trim leading spaces
    while (start != end && std::isspace(*start)) {
        start++;
    }

    // Trim trailing spaces
    while (end != start && std::isspace(*(end - 1))) {
        end--;
    }

    return std::string(start, end);  // Return the trimmed string
}

/**
 * Function to split a string by a delimiter
 * 
 * @param str The input string
 * @param delimiter The delimiter to split by
 * 
 * @return A vector of substrings
 */
std::vector<std::string> TextUtils::split(const std::string& str, const char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

/**
 * Function to get the first sentence from a text
 * The first sentence is defined as the text before the first split character
 * The split characters are defined in the splits vector
 * 
 * @param text The input text
 * @return The first sentence
 * 
 */
std::string TextUtils::get_first_sentence(const std::string& text) {
    std::string pattern = "[" + std::accumulate(TextUtils::splits.begin(), TextUtils::splits.end(), std::string("|")) + "]";
    std::regex re(pattern);
    std::sregex_token_iterator iter(text.begin(), text.end(), re, -1);
    std::string first = iter->str();

    return first;
}

/**
 * Function to split the text into sentences
 * 
 * @param text The input text
 * @return A vector of sentences
 */
std::vector<std::string> TextUtils::cut4(const std::string& text) {
    std::string trimed_text = TextUtils::trim_char(text, '\n');
    trimed_text = TextUtils::trim_char(trimed_text, '.');
    std::vector<std::string> result = TextUtils::split(trimed_text, '.');

    // filter empty strings
    result.erase(std::remove_if(result.begin(), result.end(), [](const std::string& s) { return s.empty(); }), result.end());

    // trim leading and trailing spaces
    for (auto& sentence : result) {
        sentence = TextUtils::trim(sentence);
    }

    return result;
}

std::vector<std::string> TextUtils::merge_short_sentences(std::vector<std::string> sentences, const int threshold) {
    std::vector<std::string> result;

    std::string merged_text = "";
    for (const auto& sentence : sentences) {
        if (!merged_text.empty()) {
            merged_text += " ";
        }

        merged_text += sentence;
        if (merged_text.size() >= threshold) {
            result.push_back(merged_text);
            merged_text = "";
        }
    }

    if (!merged_text.empty()) {
        result.push_back(merged_text);
    }

    return result;
}

/**
 * Function to check if a character is a split character
 */
bool TextUtils::is_split(const char c) {
    return std::find(TextUtils::splits.begin(), TextUtils::splits.end(), std::string(1, c)) != TextUtils::splits.end();
}

bool TextUtils::is_split(std::string str) {
    return std::find(TextUtils::splits.begin(), TextUtils::splits.end(), str) != TextUtils::splits.end();
}
