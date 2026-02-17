#include "distributed_key_value_store/kv_store/kv_store.h"
#include <shared_mutex>

namespace distributed_key_value_store::kv_store {
class SharedKVStore final {
public:
  [[nodiscard]] std::expected<std::string, std::string>
  get(const std::string &key) const;

  void put(const std::string &key, const std::string &value);

  void put(std::string &&key, std::string &&value);

  void remove(const std::string &key);

private:
  KVStore store;
  mutable std::shared_mutex rw_lock;
};
} // namespace distributed_key_value_store::kv_store
