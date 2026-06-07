#include <algorithm>
#include <any>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class subs_node_t {
      public:
        uint32_t id;
        std::function<void(const std::any &)> callback;
};

class broker {
      private:
        std::unordered_map<std::string, std::vector<subs_node_t>> topics;
        uint32_t token = 0;

      public:
        broker() = default;
        ~broker() = default;

        uint32_t subscribe(const std::string &topic_name, std::function<void(const std::any &)> callback);
        void unsubscribe(std::string topic_name, uint32_t token);
        void publish(std::string topic_name, std::any payload);
};

uint32_t broker::subscribe(const std::string &topic_name, std::function<void(const std::any &)> callback) {
        uint32_t new_token = ++token;
        topics[topic_name].push_back({new_token, callback});
        return new_token;
}

void broker::unsubscribe(std::string topic_name, uint32_t token) {
        auto it = topics.find(topic_name);

        if (it != topics.end()) {
                auto &vec = it->second;
                vec.erase(std::remove_if(vec.begin(), vec.end(),
                                         [token](const subs_node_t &node) { return node.id == token; }),
                          vec.end());
                if (vec.empty()) {
                        topics.erase(topic_name);
                }
        }
}

void broker::publish(std::string topic_name, std::any payload) {
        auto it = topics.find(topic_name);

        if (it == topics.end()) {
                return;
        }

        for (const auto &c : it->second) {
                c.callback(payload);
        }
}
