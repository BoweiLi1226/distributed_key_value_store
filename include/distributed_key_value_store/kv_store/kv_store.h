#include <expected>
#include <string>
#include <string_view>
#include <unordered_map>

namespace distributed_key_value_store::kv_store {
class KVStore final {
public:
  [[nodiscard]] std::expected<std::string, std::string>
  get(const std::string &key) const;

  void put(std::string_view key, std::string_view value);

  void put(std::string &&key, std::string &&value);

private:
  std::unordered_map<std::string, std::string> data;
};
} // namespace distributed_key_value_store::kv_store
