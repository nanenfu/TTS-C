#include <numeric>
#include <regex>
#include <sstream>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

#include "text_utils.h"

const std::vector<char32_t> TextUtils::delimiters = {U'，', U'。', U'？', U'！', U',', U'.', U'?', U'!', U'~', U':', U'：', U'—', U'…'};

/**
 * Function to trim leading and trailing characters
 *
 * @param str The input string
 * @param c The character to trim
 */
void TextUtils::trim_char(std::string& str, const char c) {
    // Find the first non-c character from the start
    auto start = str.find_first_not_of(c);

    // If the entire string is made up of the character 'c', clear the string
    if (start == std::string::npos) {
        str.clear();
        return;
    }

    // Find the first non-c character from the end
    auto end = str.find_last_not_of(c);

    // Erase the trailing characters
    str.erase(end + 1);  // Remove everything after the last non-c character

    // Erase the leading characters
    str.erase(0, start);  // Remove everything before the first non-c character
}

/**
 * Function to trim leading and trailing whitespace
 *
 * @param str The input string
 */
void TextUtils::trim(std::string& str) {
    // Find the first non-whitespace character from the start
    auto start = str.begin();
    while (start != str.end() && std::isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }

    // If the entire string is whitespace, clear it
    if (start == str.end()) {
        str.clear();
        return;
    }

    // Find the last non-whitespace character from the end
    auto end = str.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
        --end;
    }

    // Erase trailing whitespace
    str.erase(end, str.end());

    // Erase leading whitespace
    str.erase(str.begin(), start);
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
 * The split characters are defined in the delimiters vector
 *
 * @param text The input text
 * @return The first sentence
 *
 * @return The first sentence
 */
std::string TextUtils::get_first_sentence(const std::string& text) {
    // convert the text to a u32string
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32_text = converter.from_bytes(text);

    // find the first split character
    auto first = std::find_first_of(u32_text.begin(), u32_text.end(), TextUtils::delimiters.begin(), TextUtils::delimiters.end());

    // substring the text to get the first sentence
    std::u32string first_sentence = u32_text.substr(0, first - u32_text.begin());

    return converter.to_bytes(first_sentence);
}

/**
 * Function to join strings with a separator
 * adapted from https://stackoverflow.com/a/71983949/275339
 *
 * @param sep The separator
 * @param strings A vector of strings to join
 *
 * @return The joined string
 */
std::string TextUtils::join(const std::string& sep, const std::vector<std::string>& strings) {
    std::string result;
    for (const auto& str : strings) {
        if (!result.empty()) {
            result += sep;
        }
        result += str;
    }

    return result;
}

/**
 * Function to split the text into sentences
 *
 * @param text The input text
 * @return A vector of sentences
 */
void TextUtils::cut4(std::string& text) {
    TextUtils::trim_char(text, '\n');
    TextUtils::trim_char(text, '.');

    std::vector<std::string> sentences = TextUtils::split(text, '.');

    // join sentences with the dot
    text = TextUtils::join("\n", sentences);
}

/**
 * Function to merge short sentences into longer sentences
 *
 * @param sentences The input sentences
 * @param threshold The threshold to merge the sentences
 * @return A vector of merged sentences
 */
std::vector<std::string> TextUtils::merge_short_sentences(std::vector<std::string> sentences, const unsigned int threshold) {
    std::vector<std::string> result;

    // variable to accumulate the text sentences by sentences
    // until the threshold is reached
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
 *
 * @param c The character to check
 * @return True if the character is a split character, false otherwise
 */
bool TextUtils::is_delimiter(const char c) {
    // convert char to char32_t
    char32_t c32 = static_cast<char32_t>(c);
    return is_delimiter(c32);
}

bool TextUtils::is_delimiter(const char32_t c) {
    return std::find(TextUtils::delimiters.begin(), TextUtils::delimiters.end(), c) != TextUtils::delimiters.end();
}

/**
 * Function to split a big text into smaller segments
 *
 * @param text The input text
 * @param max_len The maximum length of each segment
 *
 * @return A vector of smaller segments
 */
std::vector<std::string> TextUtils::split_long_sentences(std::vector<std::string> sentences, const unsigned int max_len) {
    std::vector<std::string> result;

    for (const auto& sentence : sentences) {
        if (sentence.size() <= max_len) {
            result.push_back(sentence);
        } else {
            // split sentence into smaller segments by the delimiters
            std::vector<std::string> segments = TextUtils::split_by_delimiters(sentence);
            for (const auto& segment : segments) {
                if (result.empty() || result.back().size() + segment.size() > max_len) {
                    result.push_back(segment);
                } else {
                    result.back() += segment;
                }
            }

        }
    }

    return result;
}

/**
 * Function to replace all occurrences of a substring with another substring
 *
 * @param str The input string
 * @param from The substring to replace
 * @param to The substring to replace with
 */
void TextUtils::replace_all(std::string &str, const std::string &toReplace, const std::string &replaceWith) {
    size_t pos = str.find(toReplace);

    while (pos != std::string::npos) {
        str.replace(pos, toReplace.length(), replaceWith);
        pos = str.find(toReplace, pos + replaceWith.length());
    }
}

/**
 * Check if a string contains only whitespace characters
 *
 * @param str The input string
 */
bool TextUtils::is_empty(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isspace(c); });
}

/**
 * Function to split a string by delimiters
 *
 * @param str The input string
 *
 * @return A vector of substrings
 */
std::vector<std::string> TextUtils::split_by_delimiters(const std::string& str) {
    std::vector<std::string> result;
    std::string current_segment;

    for (char c : str) {
        current_segment += c;  // Add character to current chunk

        // If the current character is a delimiter, add the chunk to the result and reset the chunk
        if (TextUtils::is_delimiter(c)) {
            result.push_back(current_segment);
            current_segment.clear();
        }
    }

    return result;
}