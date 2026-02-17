#include <expected>
#include <string>
#include <unordered_map>

namespace distributed_key_value_store::kv_store {

class KVStore final {
public:
  [[nodiscard]] std::expected<std::string, std::string>
  get(const std::string &key) const;

  void put(const std::string &key, const std::string &value);

  void put(std::string &&key, std::string &&value);

  void remove(const std::string &key);

private:
  std::unordered_map<std::string, std::string> data;
};

} // namespace distributed_key_value_store::kv_store
