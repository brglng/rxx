#include <cstdio>
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

class Visitor {
    size_t m_index;

public:
    Visitor() : m_index(0) {}

    size_t index() { return m_index; }

    void operator()(int) {
        m_index = 1;
    }

    void operator()(float) {
        m_index = 2;
    }

    void operator()(void*) {
        m_index = 3;
    }
};

TEST_CASE("Var visit works") {
    Var<int, float, void*> v;
    Visitor visitor;

    v = 123;
    visit(visitor, v);
    REQUIRE(visitor.index() == v.index() + 1);

    v = 1.0f;
    visit(visitor, v);
    REQUIRE(visitor.index() == v.index() + 1);

    v = nullptr;
    visit(visitor, v);
    REQUIRE(visitor.index() == v.index() + 1);
}

TEST_CASE("match works") {
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
