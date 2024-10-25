#pragma once

#include <vector>
#include <string>
#include <codecvt>
#include <locale>

class TextUtils {
public:
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    static const std::vector<char32_t> delimiters;
    static void trim_char(std::string& str, char c);
    static void trim(std::string& str);
    static std::vector<std::string> split(const std::string& str, const char separator);
    static std::vector<std::string> split(const std::string& str, const char32_t separator);
    static std::vector<std::string> split_by_delimiters(const std::string& str);
    static std::string get_first_sentence(const std::string& text);
    static void cut4(std::string& text);
    static std::vector<std::string> merge_short_sentences(std::vector<std::string> sentences, const unsigned int threshold);
    static std::vector<std::string> split_long_sentences(std::vector<std::string> sentences, const unsigned int max_len=510);
    static bool is_delimiter(const char c);
    static bool is_delimiter(const char32_t c);
    static bool is_empty(const std::string& str);
    static void replace_all(std::string &str, const std::string &toReplace, const std::string &replaceWith);
    static std::string join(const std::string& sep, const std::vector<std::string>& strings);
};
