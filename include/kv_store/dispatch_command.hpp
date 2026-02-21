#pragma once
#include "kv_command.pb.h"
#include "utils.h"
#include <concepts>
#include <expected>

namespace distributed_key_value_store::kv_store {

template <typename T>
concept IsKVStore = requires(T s, std::string str) {
  { s.put(str, str) } -> std::same_as<std::expected<Result, Error>>;
  {
    s.put(std::move(str), std::move(str))
  } -> std::same_as<std::expected<Result, Error>>;
  { s.get(str) } -> std::same_as<std::expected<Result, Error>>;
  { s.remove(str) } -> std::same_as<std::expected<Result, Error>>;
};

template <IsKVStore T>
inline std::expected<Result, Error>
dispatch_command(T *store, kv_command::KVCommand &kv_command) {
  switch (kv_command.op()) {
  case kv_command::KVCommand_Op_PUT:
    if (!kv_command.has_value()) {
      return std::unexpected(Error::INVALID_INPUT);
    }
    return store->put(std::move(*kv_command.mutable_key()),
                      std::move(*kv_command.mutable_value()));
  case kv_command::KVCommand_Op_GET:
    return store->get(kv_command.key());
  case kv_command::KVCommand_Op_DELETE:
    return store->remove(kv_command.key());
  default:
    return std::unexpected(Error::INVALID_INPUT);
  }
}
} // namespace distributed_key_value_store::kv_store
