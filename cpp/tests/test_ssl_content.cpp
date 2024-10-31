#include <iostream>
#include <iomanip>
#include <catch2/catch_test_macros.hpp>

#include "ssl_content.h"

TEST_CASE("load_ssl_content", "[ssl_content]") {
    auto [ssl_content, ssl_content_shape] = load_ssl_content();

    REQUIRE(ssl_content.size() == 768*318);
}
