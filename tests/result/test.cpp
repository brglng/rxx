#include <catch2/catch.hpp>
#include "rxx/result.hpp"
#include "rxx/var.hpp"
#include "rxx/match.hpp"

using namespace rxx;

class A {};
class B {};
class C {};
class MyErr {};

Result<A, MyErr> ok_func() {
    return Ok(A{});
}

Result<B, MyErr> err_func() {
    return Err(MyErr{});
}

Result<C, MyErr> try_func() {
    auto a = RXX_TRY(ok_func());
    auto b = RXX_TRY(err_func());
    (void)a;
    (void)b;
    return Ok(C{});
}

TEST_CASE("RXX_TRY() works", "[Result]") {
    auto r = try_func();
    REQUIRE(r.is_err());
}
