#include <catch2/catch.hpp>
#include "rxx/slice.hpp"

using namespace rxx;

TEST_CASE("slice::init() works", "[Slice]") {
    auto s1 = rxx::slice::init({1, 2, 3});
    REQUIRE(s1.len() == 3);
    STATIC_REQUIRE(std::is_same<decltype(s1), rxx::Slice<const int>>::value);
    REQUIRE(s1[0] == 1);
    REQUIRE(s1[1] == 2);
    REQUIRE(s1[2] == 3);
}
