#include <cassert>
#include "rxx/core/num/i32/i32.hpp"
#include "rxx/rxx/collections/hash_map/hash_map.hpp"

int main() {
    auto m = rxx::HashMap<rxx::i32, rxx::i32>::New();
    assert(m.insert(1, 1).is_some());
    assert(m.insert(2, 2).is_some());
    assert(m.insert(3, 3).is_some());
    printf("%d\n", m.get(1).unwrap());
}
