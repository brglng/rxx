#include <catch2/catch.hpp>
#include "rxx/str.hpp"

using namespace rxx;

TEST_CASE("str::init() works", "[Slice]") {
    auto s1 = rxx::str::init("abc");
    REQUIRE(s1.len() == 3);
    STATIC_REQUIRE(std::is_same<decltype(s1), rxx::Str>::value);
    REQUIRE(s1[0] == 'a');
    REQUIRE(s1[1] == 'b');
    REQUIRE(s1[2] == 'c');

    char ss2[] = {'a', 'b', 'c'};
    auto s2 = rxx::str::init(ss2);
    STATIC_REQUIRE(std::is_same<decltype(s2), rxx::MutStr>::value);
    REQUIRE(s2[0] == 'a');
    REQUIRE(s2[1] == 'b');
    REQUIRE(s2[2] == 'c');
}
