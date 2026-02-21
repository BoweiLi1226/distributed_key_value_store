#pragma once
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/experimental/concurrent_channel.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/system/error_code.hpp>
#include <cstddef>

namespace distributed_key_value_store::primitives {
class AsyncRwLock {
private:
  boost::asio::experimental::concurrent_channel<void(boost::system::error_code)>
      channel_;
  static constexpr int MAX_READERS = 6;

  boost::asio::awaitable<void> read_lock() {
    co_await channel_.async_receive(boost::asio::use_awaitable);
  }

  void read_unlock() { channel_.try_send(boost::system::error_code{}); }

  boost::asio::awaitable<void> write_lock() {
    int acquired = 0;
    try {
      for (; acquired < MAX_READERS; acquired++) {
        co_await read_lock();
      }
    } catch (...) {
      for (; acquired > 0; acquired--) {
        read_unlock();
      }
      throw;
    }
  }

  void write_unlock() {
    for (int i = 0; i < MAX_READERS; i++) {
      read_unlock();
    }
  }

public:
  class [[nodiscard]] ReadGuard {
    friend AsyncRwLock;

  public:
    ReadGuard(ReadGuard &&other) noexcept : lock_(other.lock_) {
      other.lock_ = nullptr;
    }
    ReadGuard(const ReadGuard &other) = delete;
    ReadGuard &operator=(const ReadGuard &other) = delete;
    ~ReadGuard() {
      if (lock_ != nullptr)
        lock_->read_unlock();
    }

  private:
    AsyncRwLock *lock_;
    explicit ReadGuard(AsyncRwLock &async_rw_lock) : lock_(&async_rw_lock) {}
  };

  class [[nodiscard]] WriteGuard {
    friend AsyncRwLock;

  public:
    WriteGuard(WriteGuard &&other) noexcept : lock_(other.lock_) {
      other.lock_ = nullptr;
    }
    WriteGuard(const WriteGuard &other) = delete;
    WriteGuard &operator=(const WriteGuard &other) = delete;
    ~WriteGuard() {
      if (lock_ != nullptr)
        lock_->write_unlock();
    }

  private:
    AsyncRwLock *lock_;
    explicit WriteGuard(AsyncRwLock &async_rw_lock) : lock_(&async_rw_lock) {}
  };

  AsyncRwLock(const AsyncRwLock &async_rw_lock) = delete;
  AsyncRwLock(AsyncRwLock &&async_rw_lock) = delete;

  explicit AsyncRwLock(boost::asio::any_io_executor executor)
      : channel_(executor, MAX_READERS) {
    for (int i = 0; i < MAX_READERS; i++) {
      channel_.try_send(boost::system::error_code{});
    }
  }

  boost::asio::awaitable<ReadGuard> read() {
    co_await read_lock();
    co_return ReadGuard(*this);
  }

  boost::asio::awaitable<WriteGuard> write() {
    co_await write_lock();
    co_return WriteGuard(*this);
  }
};
} // namespace distributed_key_value_store::primitives
