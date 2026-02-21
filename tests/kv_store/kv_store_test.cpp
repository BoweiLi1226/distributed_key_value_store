#include "kv_store/kv_store.h"
#include "kv_store/utils.h"
#include <expected>
#include <gtest/gtest.h>

using distributed_key_value_store::kv_store::KVStore;

TEST(KVStoreUnitTest, BasicPutGet) {
  KVStore kv_store;
  auto _ = kv_store.put("bowei", "yushi");
  ASSERT_EQ("yushi", kv_store.get("bowei").value().value);
}
