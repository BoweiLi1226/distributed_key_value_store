#pragma once
#include "kv_command.pb.h"
#include "kv_store/kv_store.h"
#include "primitives/async_rw_lock.hpp"
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <expected>

namespace distributed_key_value_store::kv_store {
class SharedKVStore final {
public:
  explicit SharedKVStore(boost::asio::any_io_executor executor);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  get(std::string key) const;

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  put(std::string key, std::string value);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  remove(std::string key);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  process(kv_command::KVCommand kv_command);

private:
  KVStore store_;
  mutable primitives::AsyncRwLock lock_;
};
} // namespace distributed_key_value_store::kv_store
