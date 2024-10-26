#include <numeric>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

#include "text_utils.h"

namespace text_utils {
    /**
     * Function to trim leading and trailing characters
     *
     * @param str The input string
     * @param c The character to trim
     */
    void trim_char(std::string& str, const char c) {
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
    void trim(std::string& str) {
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
     * @param separator The separator to split by
     *
     * @return A vector of substrings
     */
    std::vector<std::string> split(const std::string& str, const char separator) {
        // convert the delimiter to a char32_t
        char32_t separator32 = static_cast<char32_t>(separator);
        return split(str, separator32);
    }

    std::vector<std::string> split(const std::string& str, const char32_t separator) {
        std::vector<std::string> result;
        std::u32string token;

        for (auto c : converter.from_bytes(str)) {
            if (c == separator) {
                result.push_back(converter.to_bytes(token));
                token.clear();
            } else {
                token += c;
            }
        }

        if (!token.empty()) {
            result.push_back(converter.to_bytes(token));
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
    std::string get_first_sentence(const std::string& text) {
        // convert the text to a u32string, since the delimiters could be unicode characters
        std::u32string u32_text = converter.from_bytes(text);

        // find the first split character
        auto first = std::find_first_of(u32_text.begin(), u32_text.end(), delimiters.begin(), delimiters.end());

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
    std::string join(const std::string& sep, const std::vector<std::string>& strings) {
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
    void cut4(std::string& text) {
        trim_char(text, '\n');
        trim_char(text, '.');

        std::vector<std::string> sentences = split(text, '.');

        // join sentences with the dot
        text = join("\n", sentences);
    }

    /**
     * Function to merge short sentences into longer sentences
     *
     * @param sentences The input sentences
     * @param threshold The threshold to merge the sentences
     * @return A vector of merged sentences
     */
    std::vector<std::string> merge_short_sentences(const std::vector<std::string>& sentences, const unsigned int threshold) {
        std::vector<std::string> result;

        // variable to accumulate the text sentences by sentences
        // until the threshold is reached
        std::string merged_text;

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
    bool is_delimiter(const char c) {
        // convert char to char32_t
        char32_t c32 = static_cast<char32_t>(c);
        return is_delimiter(c32);
    }

    bool is_delimiter(const char32_t c) {
        return std::find(delimiters.begin(), delimiters.end(), c) != delimiters.end();
    }

    /**
     * Function to split a big text into smaller segments
     *
     * @param text The input text
     * @param max_len The maximum length of each segment
     *
     * @return A vector of smaller segments
     */
    std::vector<std::string> split_long_sentences(const std::vector<std::string>& sentences, const unsigned int max_len) {
        std::vector<std::string> result;

        for (const auto& sentence : sentences) {
            if (sentence.size() <= max_len) {
                result.push_back(sentence);
            } else {
                // split sentence into smaller segments by the delimiters
                std::vector<std::string> segments = split_by_delimiters(sentence);
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
    void replace_all(std::string &str, const std::string &toReplace, const std::string &replaceWith) {
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
    bool is_empty(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](char c) { return std::isspace(c); });
    }

    /**
     * Function to split a string by delimiters
     *
     * @param str The input string
     *
     * @return A vector of substrings
     */
    std::vector<std::string> split_by_delimiters(const std::string& str) {
        std::vector<std::string> result;
        std::u32string current_segment;
        std::u32string u32_str = converter.from_bytes(str);

        for (auto c : u32_str) {
            current_segment += c;  // Add character to current chunk

            // If the current character is a delimiter, add the chunk to the result and reset the chunk
            if (is_delimiter(c)) {
                result.push_back(converter.to_bytes(current_segment));
                current_segment.clear();
            }
        }

        return result;
    }
}