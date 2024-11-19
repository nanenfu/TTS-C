#pragma once

#include <vector>
#include <tuple>
#include <string>

std::tuple<std::vector<float>, std::vector<int64_t>> load_ssl_content(const std::string& ssl_content_path);
