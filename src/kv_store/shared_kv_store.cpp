#include "distributed_key_value_store/kv_store/shared_kv_store.h"
#include "distributed_key_value_store/kv_store/dispatch_command.hpp"
#include "kv_command.pb.h"
#include <expected>
#include <mutex>
#include <shared_mutex>
#include <utility>

namespace distributed_key_value_store::kv_store {

std::expected<Result, Error> SharedKVStore::get(const std::string &key) const {
  std::shared_lock<std::shared_mutex> lock(rw_lock);
  return store.get(key);
}

std::expected<Result, Error> SharedKVStore::put(std::string key,
                                                std::string value) {
  std::lock_guard<std::shared_mutex> lock(rw_lock);
  return store.put(std::move(key), std::move(value));
}

std::expected<Result, Error> SharedKVStore::remove(const std::string &key) {
  std::lock_guard<std::shared_mutex> lock(rw_lock);
  return store.remove(key);
}

std::expected<Result, Error>
KVStore::process(kv_command::KVCommand &kv_command) {
  return dispatch_command(this, kv_command);
}
} // namespace distributed_key_value_store::kv_store
