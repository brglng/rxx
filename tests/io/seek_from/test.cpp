#include <catch2/catch.hpp>
#include "rxx/io.hpp"
#include "rxx/match.hpp"
using rxx::io::SeekFrom;

TEST_CASE("visit and match works", "[io::SeekFrom]") {
    int type = 0;
    std::uint64_t u64offset = 0;
    std::int64_t i64offset = 0;

    auto sf = SeekFrom(SeekFrom::Start(123));
    rxx::match(sf,
        [&](SeekFrom::Start const& s) {
            type = 1;
            u64offset = s.offset();
        },
        [&](SeekFrom::End const& e) {
            type = 2;
            i64offset = e.offset();
        },
        [&](SeekFrom::Current const& c) {
            type = 3;
            i64offset = c.offset();
        }
    );
    REQUIRE(type == 1);
    REQUIRE(u64offset == 123);

    sf = SeekFrom::End(123);
    rxx::match(sf,
        [&](SeekFrom::Start const& s) {
            type = 1;
            u64offset = s.offset();
        },
        [&](SeekFrom::End const& e) {
            type = 2;
            i64offset = e.offset();
        },
        [&](SeekFrom::Current const& c) {
            type = 3;
            i64offset = c.offset();
        }
    );
    REQUIRE(type == 2);
    REQUIRE(i64offset == 123);

    sf = SeekFrom::Current(456);
    rxx::match(sf,
        [&](SeekFrom::Start const& s) {
            type = 1;
            u64offset = s.offset();
        },
        [&](SeekFrom::End const& e) {
            type = 2;
            i64offset = e.offset();
        },
        [&](SeekFrom::Current const& c) {
            type = 3;
            i64offset = c.offset();
        }
    );
    REQUIRE(type == 3);
    REQUIRE(i64offset == 456);
}
