#include "kv_store/shared_kv_store.h"
#include "kv_command.pb.h"
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <expected>
#include <utility>

namespace distributed_key_value_store::kv_store {
SharedKVStore::SharedKVStore(boost::asio::any_io_executor executor)
    : strand_(boost::asio::make_strand(executor)) {}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::get(std::string key) {
  co_await boost::asio::dispatch(strand_, boost::asio::use_awaitable);
  co_return store_.get(std::move(key));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::put(std::string key, std::string value) {
  if (!strand_.running_in_this_thread()) {
    co_await boost::asio::dispatch(strand_, boost::asio::use_awaitable);
  }
  co_return store_.put(std::move(key), std::move(value));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::remove(std::string key) {
  if (!strand_.running_in_this_thread()) {
    co_await boost::asio::dispatch(strand_, boost::asio::use_awaitable);
  }
  co_return store_.remove(std::move(key));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::process(kv_command::KVCommand kv_command) {
  if (!strand_.running_in_this_thread()) {
    co_await boost::asio::dispatch(strand_, boost::asio::use_awaitable);
  }
  co_return store_.process(std::move(kv_command));
}
} // namespace distributed_key_value_store::kv_store
