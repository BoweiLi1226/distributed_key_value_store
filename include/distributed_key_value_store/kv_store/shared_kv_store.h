#pragma once
#include "distributed_key_value_store/kv_store/kv_store.h"
#include "kv_command.pb.h"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/thread_pool.hpp>
#include <expected>

namespace distributed_key_value_store::kv_store {
class SharedKVStore final {
public:
  SharedKVStore(boost::asio::io_context &io_context);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  get(std::string key);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  put(std::string key, std::string value);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  remove(std::string key);

  [[nodiscard]] boost::asio::awaitable<std::expected<Result, Error>>
  process(kv_command::KVCommand kv_command);

private:
  KVStore store_;
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
};
} // namespace distributed_key_value_store::kv_store
