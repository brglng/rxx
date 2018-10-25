#ifndef __RXX_RXX_COLLECTIONS_HASH_MAP_DEF_HPP__
#define __RXX_RXX_COLLECTIONS_HASH_MAP_DEF_HPP__

#include <pthread.h>
#include <cstdlib>
#include <tuple>
#include "rxx/bool/def.hpp"
#include "rxx/usize/def.hpp"

namespace rxx {

class u8;
class i32;
class u64;
template<typename T> class Slice;
template<typename T> class Option;

namespace collections {
namespace hash_map {

class DefaultHasher {
    std::uint64_t   m_v0;
    std::uint64_t   m_v1;
    std::uint64_t   m_v2;
    std::uint64_t   m_v3;
    std::uint64_t   m_b;
    std::size_t     m_len;
    std::uint8_t    m_npending;

    static std::uint8_t s_k[16];
    static uint64_t s_k0;
    static uint64_t s_k1;
    static pthread_once_t s_k_once;
    static void init_seed();
    static void init_seed_once();
public:
    DefaultHasher(std::uint64_t v0,
                  std::uint64_t v1,
                  std::uint64_t v2,
                  std::uint64_t v3,
                  std::uint64_t b,
                  std::size_t len,
                  std::uint8_t npending) :
        m_v0 { v0 }, m_v1 { v1 }, m_v2 { v2 }, m_v3 { v3 }, m_b { b }, m_len { len },
        m_npending { npending }
    {}

    static auto New() -> DefaultHasher;

    void write(Slice<const u8> msg);

    void write_i32(i32 i);

    auto finish() -> std::uint64_t;
};

class RandomState {
public:
    static auto New() -> RandomState {
        return RandomState {};
    }

    auto build_hasher() const -> DefaultHasher {
        return DefaultHasher::New();
    }
};

namespace impl {

using HashUint = usize;

class SafeHash {
    HashUint    m_hash;
public:
    explicit constexpr SafeHash(HashUint hash) : m_hash{hash} {}

    constexpr auto inspect() const -> HashUint {
        return m_hash;
    }

    static constexpr auto New(u64 hash) -> SafeHash;
};

}

template<typename K, typename V>
class Entry;

template<typename K, typename V>
class OccupiedEntry;

template<typename K, typename V>
class VacantEntry;

template<typename K, typename V>
class Entry {
    Bool                m_occupied;
    K*                  mp_key;
    V*                  mp_value;
    K                   m_new_key;

public:
    explicit Entry(Bool occupied, K& key, V& value, K&& new_key) :
        m_occupied { occupied },
        mp_key { &key },
        mp_value { &value },
        m_new_key { std::forward<K>(new_key) } {}

    Entry(OccupiedEntry<K, V> oe) :
        m_occupied { true },
        mp_key { oe.mp_key },
        mp_value { oe.mp_value },
        m_new_key { std::forward<K>(oe.m_new_key) }
    {}

    Entry(VacantEntry<K, V> ve) :
        m_occupied { false },
        mp_key { ve.mp_key },
        mp_value { ve.mp_value },
        m_new_key { std::forward<K>(ve.m_new_key) }
    {}

    static auto Occupied(OccupiedEntry<K, V> oe) -> Entry<K, V> {
        return Entry<K, V> { oe };
    }

    static auto Vacant(VacantEntry<K, V> ve) -> Entry<K, V> {
        return Entry<K, V> { ve };
    }

    auto is_occupied() const -> Bool { return m_occupied; }

    auto or_insert(V&& default_) -> V& {
        if (!m_occupied) {
            *mp_value = std::forward<V>(default_);
        }
        return *mp_value;
    }

    template<typename F>
    auto or_insert_with(F default_) -> V& {
        if (!m_occupied) {
            *mp_value = std::forward<V>(default_());
        }
        return *mp_value;
    }

    auto key() const -> K const& {
        return *mp_key;
    }

    template<typename F>
    auto and_modify(F f) -> Entry<K, V> {
        if (m_occupied) {
            f(*mp_value);
            return OccupiedEntry<K, V> { *mp_key, *mp_value, std::forward<V>(*mp_value) };
        }
        return *this;
    }

    template<typename F>
    auto or_default() -> Entry<K, V> {
        if (!m_occupied) {
            *mp_value = V {};
        }
        return *mp_value;
    }
};

template<typename K, typename V>
class OccupiedEntry {
    K*  mp_key;
    V*  mp_value;
    K   m_new_key;

public:
    friend class Entry<K, V>;

    OccupiedEntry(K& k, V& v, K&& new_k) :
        mp_key { &k }, mp_value { &v }, m_new_key { std::forward<K>(new_k) } {}

    auto key() const -> K const& { return *mp_key; }

    // auto remove_entry() -> std::pair<K, V> {
    // }

    auto get() -> V& { return *mp_value; }

    auto get_const() const -> V const& { return *mp_value; }

    auto into() -> V& { return *mp_value; }

    auto insert(V&& value) -> V {
        *mp_value = std::forward<V>(value);
    }

    // auto remove() -> V
};

template<typename K, typename V>
class VacantEntry {
    K*  mp_key;
    V*  mp_value;
    K   m_new_key;

public:
    friend class Entry<K, V>;

    VacantEntry(K& k, V& v, K&& new_k) :
        mp_key { &k }, mp_value { &v }, m_new_key { std::forward<K>(new_k) } {}

    auto key() const -> K const& { return *mp_key; }
    auto into_key() -> K { return *std::move(mp_key); }

    auto insert(V&& value) -> V& {
        *mp_value = std::forward<V>(value);
        return *mp_value;
    }
};

}

template<typename K, typename V, typename S = hash_map::RandomState>
class HashMap {
    static constexpr std::size_t MIN_CAPACITY = 32;

    hash_map::impl::HashUint    *m_hashes;
    std::pair<K, V>             *m_buckets;
    usize                       m_mask;
    usize                       m_len;
    S                           m_hash_builder;

public:
    HashMap(hash_map::impl::HashUint *hashes, std::pair<K, V> *buckets,
            usize mask, usize len, S&& hash_builder) :
        m_hashes { hashes },
        m_buckets { buckets },
        m_mask { mask },
        m_len { len },
        m_hash_builder { std::forward<S>(hash_builder) }
    {}

    HashMap(HashMap&& that) :
        m_hashes { that.m_hashes },
        m_buckets { that.m_buckets },
        m_mask { that.m_mask },
        m_len { that.m_len },
        m_hash_builder { std::move(that.m_hash_builder) }
    {}

    auto operator=(HashMap&& that) -> HashMap<K, V, S>& {
        if (this != &that) {
            m_hashes = that.m_hashes;
            m_buckets = that.m_buckets;
            m_mask = that.m_mask;
            m_len = that.m_len;
            m_hash_builder = std::move(that.m_hash_builder);
            that.m_hashes = nullptr;
        }
        return *this;
    }

    static auto New() -> HashMap<K, V, hash_map::RandomState>;

private:
    auto bucket_distance(usize i) const -> usize {
        return (i + m_mask + 1 - (m_hashes[i.as_size_t()] & m_mask)) & m_mask;
    }

    auto lookup_bucket(K const& key) -> Option<std::pair<K, V>&>;
    void internal_rehash(usize new_cap);
    void check_rehash();
public:
    void reserve(usize additional);

    void shrink_to_fit();

    auto entry(K&& key) -> hash_map::Entry<K, V>;

    auto len() const -> usize { return m_len; }

    auto is_empty() const -> Bool { return m_len == 0; }

    auto get(K const& k) -> Option<V&>;

    auto insert(K&& k, V&& v) -> Option<V>;

    auto remove(K const& k) -> Option<V>;

    ~HashMap() {
        if (m_hashes) {
            for (std::size_t i = 0; i < m_mask + 1; ++i) {
                if (m_hashes[i] != 0) {
                    m_buckets[i].first.K::~K();
                    m_buckets[i].second.V::~V();
                    m_hashes[i] = 0;
                }
            }
            std::free(m_hashes);
        }
    }
};

}
}

#endif /* end of include guard: __RXX_RXX_COLLECTIONS_HASH_MAP_DEF_HPP__ */
