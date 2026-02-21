#include "kv_store/shared_kv_store.h"
#include "kv_store/utils.h"
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/experimental/cancellation_condition.hpp>
#include <boost/asio/experimental/parallel_group.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>
#include <expected>
#include <format>
#include <gtest/gtest.h>
#include <vector>

using distributed_key_value_store::kv_store::SharedKVStore;

static constexpr int WORKER_THREADS = 7;
static constexpr int TASKS = 300;

TEST(SharedKVStoreTests, BasicPutGet) {
  boost::asio::thread_pool thread_pool;
  auto guard = boost::asio::make_work_guard(thread_pool);
  SharedKVStore data_store(boost::asio::make_strand(thread_pool));

  auto puts = boost::asio::co_spawn(
      thread_pool,
      [&data_store]() -> boost::asio::awaitable<void> {
        auto current_executor = co_await boost::asio::this_coro::executor;
        std::vector<boost::asio::awaitable<void>> tasks;
        tasks.reserve(TASKS);
        for (int i = 0; i < TASKS; ++i) {
          tasks.push_back(boost::asio::co_spawn(
              current_executor,
              [&data_store, i] -> boost::asio::awaitable<void> {
                auto _ =
                    co_await data_store.put(std::format("bowei{}", i), "yushi");
              },
              boost::asio::use_awaitable));
        }

        for (auto &task : tasks) {
          co_await std::move(task);
        }
      },
      boost::asio::use_future);
  puts.get();

  auto gets = boost::asio::co_spawn(
      thread_pool,
      [&data_store]() -> boost::asio::awaitable<void> {
        for (int i = 0; i < TASKS; ++i) {
          auto result = co_await data_store.get(std::format("bowei{}", i));
          EXPECT_EQ("yushi", result.value().value);
        }
      },
      boost::asio::use_future);
  gets.get();

  guard.reset();
}
