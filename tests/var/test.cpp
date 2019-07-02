#include <catch2/catch.hpp>
#include "rxx/var.hpp"
#include "rxx/match.hpp"

using namespace rxx;

TEST_CASE("Var type is correct", "[Var]") {
    Var<int, float, void*> v;
    v = 123;
    REQUIRE(v.is<int>());
    REQUIRE(v.index() == 0);
    v = 1.0f;
    REQUIRE(v.is<float>());
    REQUIRE(v.index() == 1);
    v = nullptr;
    REQUIRE(v.is<void*>());
    REQUIRE(v.index() == 2);
}

TEST_CASE("Var visit and match works", "[Var]") {
    Var<int, float, void*> v;

    size_t id = 0;

    v = 123;
    RXX_MATCH(v,
        [&id](int) { id = 1; },
        [&id](float) { id = 2; },
        [&id](void*) { id = 3; }
    );
    REQUIRE(id == v.index() + 1);

    v = 1.0f;
    RXX_MATCH(v,
        [&id](int) { id = 1; },
        [&id](float) { id = 2; },
        [&id](void*) { id = 3; }
    );
    REQUIRE(id == v.index() + 1);

    v = nullptr;
    RXX_MATCH(v,
        [&id](int) { id = 1; },
        [&id](float) { id = 2; },
        [&id](void*) { id = 3; }
    );
    REQUIRE(id == v.index() + 1);
}
