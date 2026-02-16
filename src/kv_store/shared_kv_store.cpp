#include "distributed_key_value_store/kv_store/shared_kv_store.h"
#include <mutex>
#include <shared_mutex>
#include <utility>

namespace distributed_key_value_store::kv_store {

inline std::expected<std::string, std::string>
SharedKVStore::get(const std::string &key) const {
  std::shared_lock<std::shared_mutex> lock(rw_lock);
  return store.get(key);
}

inline void SharedKVStore::put(std::string_view key, std::string_view value) {
  std::unique_lock<std::shared_mutex> lock(rw_lock);
  store.put(key, value);
}

inline void SharedKVStore::put(std::string &&key, std::string &&value) {
  std::unique_lock<std::shared_mutex> lock(rw_lock);
  store.put(std::move(key), std::move(value));
}
} // namespace distributed_key_value_store::kv_store
