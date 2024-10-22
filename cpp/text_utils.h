#pragma once

#include <vector>
#include <string>

class TextUtils {
public:
    static const std::vector<std::string> delimiters;
    static void trim_char(std::string& str, char c);
    static void trim(std::string& str);
    static std::vector<std::string> split(const std::string& str, const char delimiter);
    static std::vector<std::string> split_by_delimiters(const std::string& str);
    static std::string get_first_sentence(const std::string& text);
    static std::vector<std::string> cut4(const std::string& text);
    static std::vector<std::string> merge_short_sentences(std::vector<std::string> sentences, const unsigned int threshold);
    static std::vector<std::string> split_long_sentences(std::vector<std::string> sentences, const unsigned int max_len=510);
    static bool is_delimiter(const char c);
    static bool is_empty(const std::string& str);
    static void replace_all(std::string &str, const std::string &toReplace, const std::string &replaceWith);
};
