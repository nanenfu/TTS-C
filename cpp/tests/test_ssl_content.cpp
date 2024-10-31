#include <iostream>
#include <iomanip>
#include <catch2/catch_test_macros.hpp>

#include "ssl_content.h"

TEST_CASE("load_ssl_content", "[ssl_content]") {
    auto [ssl_content, ssl_content_shape] = load_ssl_content();

    std::cout << std::fixed << std::setprecision(8) << ssl_content[0] << std::endl;
    std::cout << std::fixed << std::setprecision(8) << ssl_content[1] << std::endl;
    std::cout << std::fixed << std::setprecision(8) << ssl_content[100] << std::endl;
    std::cout << std::fixed << std::setprecision(8) << ssl_content[1000] << std::endl;

    REQUIRE(ssl_content.size() == 768*318);
}
