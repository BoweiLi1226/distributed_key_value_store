#pragma once
#include "utils.h"
#include <expected>
#include <string>
#include <unordered_map>

namespace distributed_key_value_store::kv_store {
class KVStore final {
public:
  [[nodiscard]] std::expected<Result, Error> get(std::string key) const;

  [[nodiscard]] std::expected<Result, Error> put(std::string key,
                                                 std::string value);

  [[nodiscard]] std::expected<Result, Error> remove(std::string key);

private:
  std::unordered_map<std::string, std::string> data_;
};

} // namespace distributed_key_value_store::kv_store
