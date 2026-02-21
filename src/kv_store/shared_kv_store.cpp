#include "kv_store/shared_kv_store.h"
#include "kv_command.pb.h"
#include "primitives/async_rw_lock.hpp"
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <expected>
#include <utility>

namespace distributed_key_value_store::kv_store {

SharedKVStore::SharedKVStore(boost::asio::any_io_executor executor)
    : lock_(executor) {}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::get(std::string key) const {
  primitives::AsyncRwLock::ReadGuard guard = co_await lock_.read();
  co_return store_.get(std::move(key));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::put(std::string key, std::string value) {
  primitives::AsyncRwLock::WriteGuard guard = co_await lock_.write();
  co_return store_.put(std::move(key), std::move(value));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::remove(std::string key) {
  primitives::AsyncRwLock::WriteGuard guard = co_await lock_.write();
  co_return store_.remove(std::move(key));
}

boost::asio::awaitable<std::expected<Result, Error>>
SharedKVStore::process(kv_command::KVCommand kv_command) {
  switch (kv_command.op()) {
  case kv_command::KVCommand_Op_PUT:
    if (!kv_command.has_value()) {
      co_return std::unexpected(Error::INVALID_INPUT);
    }
    co_return store_.put(std::move(*kv_command.mutable_key()),
                         std::move(*kv_command.mutable_value()));
  case kv_command::KVCommand_Op_GET:
    co_return store_.get(kv_command.key());
  case kv_command::KVCommand_Op_DELETE:
    co_return store_.remove(kv_command.key());
  default:
    co_return std::unexpected(Error::INVALID_INPUT);
  }
}
} // namespace distributed_key_value_store::kv_store
