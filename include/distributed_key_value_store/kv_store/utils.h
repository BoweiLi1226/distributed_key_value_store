#pragma once
#include <string>

namespace distributed_key_value_store::kv_store {
struct Result {
  std::string value;
};

enum class Error {
  KEY_DOES_NOT_EXIST,
  DUPLICATE_KEY_VALUE_PAIR,
  INVALID_INPUT,
};
} // namespace distributed_key_value_store::kv_store
