#pragma once

#include <vector>
#include <string>

class TextUtils {
public:
    static const std::vector<std::string> splits;
    static std::string trim_char(const std::string& str, char c);
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, const char delimiter);
    static std::string get_first_sentence(const std::string& text);
    static std::vector<std::string> cut4(const std::string& text);
    static std::vector<std::string> merge_short_sentences(std::vector<std::string> sentences, const int threshold);
    static bool is_split(const char c);
    static bool is_split(std::string str);
};
