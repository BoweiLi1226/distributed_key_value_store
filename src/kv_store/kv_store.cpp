#include "distributed_key_value_store/kv_store/kv_store.h"
#include "distributed_key_value_store/kv_store/utils.h"
#include "kv_command.pb.h"
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

std::expected<Result, Error>
KVStore::process(kv_command::KVCommand kv_command) {
  switch (kv_command.op()) {
  case kv_command::KVCommand_Op_PUT:
    if (!kv_command.has_value()) {
      return std::unexpected(Error::INVALID_INPUT);
    }
    return this->put(std::move(*kv_command.mutable_key()),
                     std::move(*kv_command.mutable_value()));
  case kv_command::KVCommand_Op_GET:
    return this->get(kv_command.key());
  case kv_command::KVCommand_Op_DELETE:
    return this->remove(kv_command.key());
  default:
    return std::unexpected(Error::INVALID_INPUT);
  }
}
} // namespace distributed_key_value_store::kv_store
