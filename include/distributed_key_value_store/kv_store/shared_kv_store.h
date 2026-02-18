#pragma once
#include "distributed_key_value_store/kv_store/kv_store.h"
#include "kv_command.pb.h"
#include <expected>
#include <shared_mutex>

namespace distributed_key_value_store::kv_store {
class SharedKVStore final {
public:
  [[nodiscard]] std::expected<Result, Error> get(const std::string &key) const;

  [[nodiscard]] std::expected<Result, Error> put(std::string key,
                                                 std::string value);

  [[nodiscard]] std::expected<Result, Error> remove(const std::string &key);

  [[nodiscard]] std::expected<Result, Error>
  process(const kv_command::KVCommand &kv_command);

private:
  KVStore store;
  mutable std::shared_mutex rw_lock;
};
} // namespace distributed_key_value_store::kv_store
