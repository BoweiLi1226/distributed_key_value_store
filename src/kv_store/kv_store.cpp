#include "distributed_key_value_store/kv_store/kv_store.h"
#include "distributed_key_value_store/kv_store/dispatch_command.hpp"
#include "distributed_key_value_store/kv_store/utils.h"
#include "kv_command.pb.h"
#include <expected>

namespace distributed_key_value_store::kv_store {

std::expected<Result, Error> KVStore::get(const std::string &key) const {
  auto it = data.find(key);
  if (it != data.end()) {
    return Result(it->second);
  } else {
    return std::unexpected(Error::KEY_DOES_NOT_EXIST);
  }
}

std::expected<Result, Error> KVStore::put(std::string key, std::string value) {
  auto it = data.find(key);
  if (it != data.end()) {
    if (it->second == value) {
      return std::unexpected(Error::DUPLICATE_KEY_VALUE_PAIR);
    } else {
      it->second = std::move(value);
      return Result(it->second);
    }
  } else {
    auto [new_it, _] = data.emplace(std::move(key), std::move(value));
    return Result(new_it->second);
  }
}

std::expected<Result, Error> KVStore::remove(const std::string &key) {
  auto it = data.find(key);
  if (it != data.end()) {
    Result result(it->second);
    data.erase(it);
    return result;
  } else {
    return std::unexpected(Error::KEY_DOES_NOT_EXIST);
  }
}

std::expected<Result, Error>
KVStore::process(kv_command::KVCommand &kv_command) {
  return dispatch_command(this, kv_command);
}
} // namespace distributed_key_value_store::kv_store
