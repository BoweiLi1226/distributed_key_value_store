#include "kv_store/kv_store.h"
#include "kv_store/utils.h"
#include <expected>

namespace distributed_key_value_store::kv_store {

std::expected<Result, Error> KVStore::get(std::string key) const {
  auto it = data_.find(key);
  if (it != data_.end()) {
    return Result(it->second);
  } else {
    return std::unexpected(Error::KEY_DOES_NOT_EXIST);
  }
}

std::expected<Result, Error> KVStore::put(std::string key, std::string value) {
  auto it = data_.find(key);
  if (it != data_.end()) {
    if (it->second == value) {
      return std::unexpected(Error::DUPLICATE_KEY_VALUE_PAIR);
    } else {
      it->second = std::move(value);
      return Result(it->second);
    }
  } else {
    auto [new_it, _] = data_.emplace(std::move(key), std::move(value));
    return Result(new_it->second);
  }
}

std::expected<Result, Error> KVStore::remove(std::string key) {
  auto it = data_.find(key);
  if (it != data_.end()) {
    Result result(it->second);
    data_.erase(it);
    return result;
  } else {
    return std::unexpected(Error::KEY_DOES_NOT_EXIST);
  }
}
} // namespace distributed_key_value_store::kv_store
