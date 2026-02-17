#include "distributed_key_value_store/kv_store/kv_store.h"
#include <format>

namespace distributed_key_value_store::kv_store {
inline std::expected<std::string, std::string>
KVStore::get(const std::string &key) const {
  auto it = data.find(key);
  if (it != data.end()) {
    return it->second;
  } else {
    return std::unexpected(std::format("Key {} does not exist.", key));
  }
}

inline void KVStore::put(const std::string &key, const std::string &value) {
  data.emplace(key, value);
}

inline void KVStore::put(std::string &&key, std::string &&value) {
  data.emplace(std::move(key), std::move(value));
}

inline void KVStore::remove(const std::string &key) { data.erase(key); }
} // namespace distributed_key_value_store::kv_store
