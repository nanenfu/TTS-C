#include <iostream>
#include <iomanip>
#include <catch2/catch_test_macros.hpp>

#include "ssl_content.h"

TEST_CASE("load_ssl_content", "[ssl_content]") {
    const std::string ssl_content_path { "ssl_content.npy" };
    auto [ssl_content, ssl_content_shape] = load_ssl_content(ssl_content_path);

    REQUIRE(ssl_content.size() == 768*318);
}
