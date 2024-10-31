#pragma once

#include <vector>
#include <tuple>
#include <string>

const std::string ssl_content_path { "ssl_content.npy" };
std::tuple<std::vector<float>, std::vector<int64_t>> load_ssl_content();
