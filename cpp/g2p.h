#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class G2P {
public:
    G2P(const std::string dict_file);
    std::vector<std::string> translate(std::string token, const std::string lang);

private:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> g2p_en_dict;
};
