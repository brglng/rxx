#include "rxx/collections/hash_map/def.hpp"

namespace rxx {
namespace collections {
namespace hash_map {

std::uint8_t DefaultHasher::s_k[16] = {0};
std::uint64_t DefaultHasher::s_k0;
std::uint64_t DefaultHasher::s_k1;
pthread_once_t DefaultHasher::s_k_once = PTHREAD_ONCE_INIT;

}
}
}
