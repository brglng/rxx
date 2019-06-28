#include <cstdio>
#include <catch2/catch.hpp>
#include "rxx/var.hpp"

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
    visit(v, visitor);
    REQUIRE(visitor.index() == v.index() + 1);

    v = 1.0f;
    visit(v, visitor);
    REQUIRE(visitor.index() == v.index() + 1);

    v = nullptr;
    visit(v, visitor);
    REQUIRE(visitor.index() == v.index() + 1);
}
