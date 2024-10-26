#pragma once

#include <vector>
#include <string>
#include <codecvt>
#include <locale>

namespace text_utils {
    bool is_delimiter(const char c);
    bool is_delimiter(const char32_t c);
    bool is_empty(const std::string& str);
    void trim_char(std::string& str, char c);
    void trim(std::string& str);
    void replace_all(std::string &str, const std::string &toReplace, const std::string &replaceWith);
    std::vector<std::string> split(const std::string& str, const char separator);
    std::vector<std::string> split(const std::string& str, const char32_t separator);
    std::string join(const std::string& sep, const std::vector<std::string>& strings);
    std::vector<std::string> split_by_delimiters(const std::string& str);
    std::string get_first_sentence(const std::string& text);
    void cut4(std::string& text);
    std::vector<std::string> merge_short_sentences(const std::vector<std::string>& sentences, const unsigned int threshold);
    std::vector<std::string> split_long_sentences(const std::vector<std::string>& sentences, const unsigned int max_len = 510);

    static const std::vector<char32_t> delimiters = {U'，', U'。', U'？', U'！', U',', U'.', U'?', U'!', U'~', U':', U'：', U'—', U'…'};
// discussed in https://stackoverflow.com/a/43153057/275339
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
#pragma GCC diagnostic pop
};
