#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Node {
      public:
        virtual ~Node() = default;
        virtual size_t get_size() const = 0;
        virtual bool is_dir() const = 0;
};

class File : public Node {
        std::string content;

      public:
        explicit File(std::string c) : content(std::move(c)) {}

        size_t get_size() const override { return content.size(); }
        bool is_dir() const override { return false; }
};

class Directory : public Node {
      public:
        std::unordered_map<std::string, std::unique_ptr<Node>> children;

        size_t get_size() const override {
                size_t total = 0;
                for (const auto &[name, node] : children) {
                        total += node->get_size();
                }
                return total;
        }

        bool is_dir() const override { return true; }
};

class VFS {
      private:
        std::unique_ptr<Directory> root;

        std::vector<std::string> split_path(const std::string &path) {
                std::vector<std::string> parts;
                std::stringstream ss(path);
                std::string part;
                while (std::getline(ss, part, '/')) {
                        if (!part.empty())
                                parts.push_back(part);
                }
                return parts;
        }

        Directory *traverse_to_parent(const std::vector<std::string> &parts) {
                Directory *current = root.get();

                for (size_t i = 0; i < parts.size() - 1; ++i) {
                        auto it = current->children.find(parts[i]);
                        if (it == current->children.end() || !it->second->is_dir()) {
                                return nullptr;
                        }

                        current = static_cast<Directory *>(it->second.get());
                }
                return current;
        }

      public:
        VFS() : root(std::make_unique<Directory>()) {}

        int mkdir(const std::string &path) {
                auto parts = split_path(path);
                if (parts.empty()) {
                        return -1;
                }

                Directory *parent = traverse_to_parent(parts);
                if (!parent) {
                        return -1;
                }

                const std::string &name = parts.back();
                if (parent->children.count(name)) {
                        return -1;
                }

                parent->children[name] = std::make_unique<Directory>();
                return 0;
        }

        int touch(const std::string &path, const std::string &content) {
                auto parts = split_path(path);
                if (parts.empty()) {
                        return -1;
                }

                Directory *parent = traverse_to_parent(parts);
                if (!parent) {
                        return -1;
                }

                const std::string &name = parts.back();
                if (parent->children.count(name)) {
                        return -1;
                }

                parent->children[name] = std::make_unique<File>(content);
                return 0;
        }

        void deleteNode(const std::string &path) {
                auto parts = split_path(path);
                if (parts.empty()) {
                        return;
                }

                Directory *parent = traverse_to_parent(parts);
                if (!parent) {
                        return;
                }

                parent->children.erase(parts.back());
        }

        size_t get_size(const std::string &path) {
                if (path == "/" || path.empty()) {
                        return root->get_size();
                }

                auto parts = split_path(path);
                Directory *parent = traverse_to_parent(parts);
                if (!parent) {
                        return 0;
                }

                const std::string &name = parts.back();
                auto it = parent->children.find(name);
                if (it == parent->children.end()) {
                        return 0;
                }

                return it->second->get_size();
        }
};

int main() {}
