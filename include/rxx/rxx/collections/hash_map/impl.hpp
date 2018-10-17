#ifndef __RXX_RXX_COLLECTIONS_HASH_MAP_IMPL_HPP__
#define __RXX_RXX_COLLECTIONS_HASH_MAP_IMPL_HPP__

#include <pthread.h>
#include <cstdio>
#include <cassert>
#include <ctime>
#include "rxx/alloc/global/def.hpp"
#include "rxx/core/num/u8/def.hpp"
#include "rxx/core/num/i32/def.hpp"
#include "rxx/core/slice/def.hpp"
#include "rxx/core/option/def.hpp"
#include "rxx/core/alloc/layout/def.hpp"
#include "rxx/rxx/collections/hash_map/def.hpp"

namespace rxx {

/* Test of the CPU is Little Endian and supports not aligned accesses.
 * Two interesting conditions to speedup the function that happen to be
 * in most of x86 servers. */
#if defined(__X86_64__) || defined(__x86_64__) || defined (__i386__)
#define _RXX_UNALIGNED_LE_CPU
#endif

#define _RXX_ROTL(x, b) (std::uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define _RXX_U32TO8_LE(p, v)                \
    (p)[0] = (std::uint8_t)((v));           \
    (p)[1] = (std::uint8_t)((v) >> 8);      \
    (p)[2] = (std::uint8_t)((v) >> 16);     \
    (p)[3] = (std::uint8_t)((v) >> 24);

#define _RXX_U64TO8_LE(p, v)                                \
    _RXX_U32TO8_LE((p), (std::uint32_t)((v)));              \
    _RXX_U32TO8_LE((p) + 4, (std::uint32_t)((v) >> 32));

#ifdef _RXX_UNALIGNED_LE_CPU
#define _RXX_U8TO64_LE(p) (*((std::uint64_t*)(p)))
#else
#define _RXX_U8TO64_LE(p)                                                   \
    (((std::uint64_t)((p)[0])) | ((std::uint64_t)((p)[1]) << 8) |           \
     ((std::uint64_t)((p)[2]) << 16) | ((std::uint64_t)((p)[3]) << 24) |    \
     ((std::uint64_t)((p)[4]) << 32) | ((std::uint64_t)((p)[5]) << 40) |    \
     ((std::uint64_t)((p)[6]) << 48) | ((std::uint64_t)((p)[7]) << 56))
#endif

#define _RXX_SIPROUND                   \
    do {                                \
        m_v0 += m_v1;                   \
        m_v1 = _RXX_ROTL(m_v1, 13);     \
        m_v1 ^= m_v0;                   \
        m_v0 = _RXX_ROTL(m_v0, 32);     \
        m_v2 += m_v3;                   \
        m_v3 = _RXX_ROTL(m_v3, 16);     \
        m_v3 ^= m_v2;                   \
        m_v0 += m_v3;                   \
        m_v3 = _RXX_ROTL(m_v3, 21);     \
        m_v3 ^= m_v0;                   \
        m_v2 += m_v1;                   \
        m_v1 = _RXX_ROTL(m_v1, 17);     \
        m_v1 ^= m_v2;                   \
        m_v2 = _RXX_ROTL(m_v2, 32);     \
    } while (0)

inline void DefaultHasher::init_seed() {
    bool err = false;

    std::FILE *frand = std::fopen("/dev/urandom", "rb");
    if (!frand) {
        std::fclose(frand);
        err = true;
    }

    if (!err) {
        std::size_t n = std::fread(s_k, 1, 16, frand);
        if (n != 16) {
            err = true;
        }
        std::fclose(frand);
    }

    if (err) {
        std::srand(std::time(nullptr));
        for (int i = 0; i < 4; i++) {
            *((int*)s_k + i) = std::rand();
        }
    }

    s_k0 = _RXX_U8TO64_LE(s_k);
    s_k1 = _RXX_U8TO64_LE(s_k + 8);
}

inline void DefaultHasher::init_seed_once() {
    pthread_once(&s_k_once, &init_seed);
}

inline auto DefaultHasher::New() -> DefaultHasher {
    return DefaultHasher {
        0x736f6d6570736575ULL,
        0x646f72616e646f6dULL,
        0x6c7967656e657261ULL,
        0x7465646279746573ULL,
        0,
        0,
        0
    };
}

inline void DefaultHasher::write(Slice<const u8> msg) {
    auto in = reinterpret_cast<std::uint8_t const*>(msg.as_ptr());
    std::size_t inlen = msg.len().as<std::size_t>();
    auto end = in + inlen - ((m_npending + inlen) & 7);
    int left = (m_npending + inlen) & 7;
    m_v3 ^= s_k1;
    m_v2 ^= s_k0;
    m_v1 ^= s_k1;
    m_v0 ^= s_k0;

    if (m_npending > 0) {
        std::uint64_t m = 0;
        auto bytes_to_read = std::min(inlen, (std::size_t)8 - m_npending);
        switch (bytes_to_read) {
        case 7: m |= ((std::uint64_t)in[6]) << 48;
        case 6: m |= ((std::uint64_t)in[5]) << 40;
        case 5: m |= ((std::uint64_t)in[4]) << 32;
        case 4: m |= ((std::uint64_t)in[3]) << 24;
        case 3: m |= ((std::uint64_t)in[2]) << 16;
        case 2: m |= ((std::uint64_t)in[1]) << 8;
        case 1: m |= ((std::uint64_t)in[0]); break;
        case 0: break;
        }
        in += bytes_to_read;
        m_b |= m << ((std::uint64_t)m_npending << 3);
        m_npending += bytes_to_read;

        if (m_npending == 8) {
            m_v3 ^= m_b;

            _RXX_SIPROUND;

            m_v0 ^= m_b;

            m_b = 0;
            m_npending = 0;
        }
    }

    for (; in != end; in += 8) {
        std::uint64_t m = _RXX_U8TO64_LE(in);
        m_v3 ^= m;

        _RXX_SIPROUND;

        m_v0 ^= m;
    }

    switch (left) {
    case 7: m_b |= ((std::uint64_t)in[6]) << 48;
    case 6: m_b |= ((std::uint64_t)in[5]) << 40;
    case 5: m_b |= ((std::uint64_t)in[4]) << 32;
    case 4: m_b |= ((std::uint64_t)in[3]) << 24;
    case 3: m_b |= ((std::uint64_t)in[2]) << 16;
    case 2: m_b |= ((std::uint64_t)in[1]) << 8;
    case 1: m_b |= ((std::uint64_t)in[0]); break;
    case 0: break;
    }
}

inline void DefaultHasher::write_i32(i32 i) {
    write(slice(reinterpret_cast<u8 const*>(&i), sizeof(i32)));
}

inline auto DefaultHasher::finish() -> std::uint64_t {
#ifndef _RXX_UNALIGNED_LE_CPU
    std::uint64_t hash;
    std::uint8_t *out = (std::uint8_t*)&hash;
#endif

    m_b |= (std::uint64_t)m_npending << 56;

    m_v3 ^= m_b;

    _RXX_SIPROUND;

    m_v0 ^= m_b;
    m_v2 ^= 0xff;

    _RXX_SIPROUND;
    _RXX_SIPROUND;

    m_b = m_v0 ^ m_v1 ^ m_v2 ^ m_v3;

#ifndef _RXX_UNALIGNED_LE_CPU
    _RXX_U64TO8_LE(out, m_b);
    return hash;
#else
    return m_b;
#endif
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::New() -> HashMap<K, V, RandomState> {
    return HashMap<K, V, RandomState> {
        nullptr, nullptr, (std::uint64_t)0 - 1, 0, RandomState::New()
    };
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::lookup_bucket(K const& key) -> Option<std::pair<K, V>&> {
    auto hasher = m_hash_builder.build_hasher();
    key.hash(hasher);
    auto safe_hash = hash_map::impl::SafeHash::New(hasher.finish());
    auto hash = safe_hash.inspect();
    auto cap = m_mask + 1;
    size_t i = hash & m_mask;
    size_t dist = 0;
    while (dist < cap) {
        auto& bucket = m_buckets[i];

        if (m_hashes[i] == 0) {
            return None;
        }

        if (dist > bucket_distance(i)) {
            return None;
        }

        if (m_hashes[i] == hash && bucket.first == key) {
            return Some(bucket);
        }

        i = (i + 1) & m_mask;
        ++dist;
    }

    /* reaches here only when cap == 0 */
    return None;
}

namespace hash_map { namespace impl {

template<typename K, typename V>
auto calculate_layout(std::size_t capacity) -> Result<std::pair<Layout, usize>, LayoutErr> {
    auto hashes = RXX_RESULT_TRY(Layout::array<HashUint>(capacity));
    auto pairs = RXX_RESULT_TRY(Layout::array<std::pair<K, V>>(capacity));
    return hashes.extend(pairs).map([&hashes](std::pair<Layout, usize> result) -> std::pair<Layout, usize> {
        return std::make_pair(
            std::move(result.first),
            hashes.size() + hashes.padding_needed_for(alignof(std::pair<K, V>)));
    });
}

}}

template<typename K, typename V, typename S>
inline void HashMap<K, V, S>::internal_rehash(usize new_capacity) {
    usize capacity = m_mask + 1;

    if (new_capacity <= capacity) return;

    auto pair_layout = hash_map::impl::calculate_layout<K, V>(capacity).unwrap();
    auto layout = std::move(pair_layout.first);
    auto pairs_offset = pair_layout.second;

    auto hashes = Global.alloc(layout);
    auto buckets = static_cast<std::pair<K, V>*>(hashes + pairs_offset);

    auto new_map = HashMap<K, V, S> {
        hashes,
        buckets,
        new_capacity.wrapping_sub(1).as_size_t(),
        0,
        std::move(m_hash_builder)
    };

    for (std::size_t i = 0; i < capacity; ++i) {
        if (m_hashes[i] != 0) {
            new_map.insert(std::move(m_buckets[i].first),
                           std::move(m_buckets[i].second));
            m_hashes[i] = 0;
        }
    }

    *this = std::move(new_map);
}

template<typename K, typename V, typename S>
inline void HashMap<K, V, S>::reserve(usize additional) {
    auto new_capacity = usize {
        std::max(m_mask + 1 + additional, usize { MIN_CAPACITY })
    };

    if (!new_capacity.is_power_of_two()) {
        new_capacity = new_capacity.next_power_of_two();
    }

    internal_rehash(new_capacity);
}

template<typename K, typename V, typename S>
inline void HashMap<K, V, S>::shrink_to_fit() {
    auto new_capacity = usize {
        std::max(usize {m_len}.next_power_of_two().as_size_t(),
                 MIN_CAPACITY)
    };

    internal_rehash(new_capacity);
}

template<typename K, typename V, typename S>
inline void HashMap<K, V, S>::check_rehash() {
    std::size_t capacity = m_mask + 1;

    if (capacity == 0) {
        internal_rehash(MIN_CAPACITY);
    } else if (m_len * 10 >= capacity * 9) {
        internal_rehash(capacity * 2);
    }
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::entry(K&& k) -> hash_map::Entry<K, V> {
    check_rehash();

    auto hasher = m_hash_builder.build_hasher();
    k.hash(hasher);
    auto hash = hash_map::impl::SafeHash::New(hasher.finish()).inspect();

    std::size_t capacity = m_mask + 1;
    std::size_t i = hash & m_mask;
    std::size_t distance = 0;

    std::pair<K, V>* ret_pair = nullptr;
    Option<V> tmp_value = None;

    while (distance < capacity) {
        auto& bucket = m_buckets[i];

        if (m_hashes[i] == 0) {
            m_hashes[i] = hash;
            ++m_len;

            if (ret_pair) {
                bucket.second = std::move(tmp_value.unwrap());
                return hash_map::VacantEntry<K, V> {
                    ret_pair->first,
                    ret_pair->second,
                    std::forward<K>(k)
                };
            } else {
                return hash_map::VacantEntry<K, V> {
                    bucket.first,
                    bucket.second,
                    std::forward<K>(k)
                };
            }
        }

        if (m_hashes[i] == hash && bucket.first == k) {
            return hash_map::OccupiedEntry<K, V> {
                bucket.first,
                bucket.second,
                std::forward<K>(k)
            };
        }

        std::size_t bucket_dist = bucket_distance(i);
        if (bucket_dist < distance) {
            std::pair<K, V> tmp_bucket = std::move(bucket);

            if (!ret_pair) {
                ret_pair = &bucket;
            } else {
                bucket.second = std::move(tmp_value.unwrap());
            }

            std::swap(m_hashes[i], hash);
            k = std::move(tmp_bucket.first);
            tmp_value = std::move(tmp_bucket.second);
            distance = bucket_dist;
        }

        i = (i + 1) & m_mask;
        ++distance;
    }

    assert(0);
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::get(K const& k) -> Option<V&> {
    auto opt_bucket = lookup_bucket(k);
    if (opt_bucket.is_some()) {
        return Some(opt_bucket.unwrap().second);
    } else {
        return None;
    }
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::insert(K&& k, V&& v) -> Option<V> {
    V* value = nullptr;
    if (entry(std::forward<K>(k)).and_modify([&v](V& entry_value) {
        entry_value = std::forward<V>(v);
    }).is_occupied()) {
        return None;
    } else {
        return Some(std::move(*value));
    }
}

template<typename K, typename V, typename S>
inline auto HashMap<K, V, S>::remove(K const& k) -> Option<V> {
    auto removed = Bool { false };
    auto opt_bucket = lookup_bucket(k);
    auto result = Option<V> {};
    if (opt_bucket.is_some()) {
        std::pair<K, V> *p_bucket = &opt_bucket.unwrap();

        result = std::move(p_bucket->second);

        std::size_t distance = 0;
        std::size_t i_bucket = p_bucket - m_buckets;
        std::size_t capacity = m_mask + 1;
        std::size_t i_end = (i_bucket + 1) & m_mask;

        while (distance < capacity) {
            if (m_hashes[i_end] == 0)
                break;

            if (bucket_distance(i_end) == 0)
                break;

            i_end = (i_end + 1) & m_mask;
            ++distance;
        }

        std::size_t i = i_bucket;
        std::size_t j = (i + 1) & m_mask;
        while (j != i_end) {
            m_hashes[i] = m_hashes[j];
            m_buckets[i] = std::move(m_buckets[j]);
            i = j;
            j = (i + 1) & m_mask;
        }

        m_hashes[i] = 0;
        --m_len;

        removed = true;
    }

    return result;
}



}

#endif /* end of include guard: __RXX_RXX_COLLECTIONS_HASH_MAP_IMPL_HPP__ */
