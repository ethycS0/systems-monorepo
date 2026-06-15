#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Sample {
        double ts;
        std::vector<std::string> stack;
};

struct Event {
        std::string kind;
        double ts;
        std::string name;

        void print_debug() const {
                std::cout << "Kind: " << kind << std::endl;
                std::cout << "ts : " << ts << std::endl;
                std::cout << "Name: " << name << std::endl;
                std::cout << "---" << std::endl;
        }
};

std::vector<Event> convert_to_trace(const std::vector<Sample> &samples) {
        std::vector<std::string> ongoing;
        std::vector<Event> events;

        for (const auto &s : samples) {
                size_t common = 0;
                size_t max = std::min(ongoing.size(), s.stack.size());

                while (common < max && ongoing[common] == s.stack[common]) {
                        common += 1;
                }

                while (ongoing.size() > common) {
                        events.push_back({"End", s.ts, ongoing.back()});
                        ongoing.pop_back();
                }

                for (size_t i = common; i < s.stack.size(); ++i) {
                        events.push_back({"Start", s.ts, s.stack[i]});
                        ongoing.push_back(s.stack[i]);
                }
        }

        return events;
}

int main() {
        Sample s1({1, {"main"}});
        Sample s2({3.5, {"main", "calc"}});
        Sample s3({7.25, {"main", "calc", "add"}});
        Sample s4({10, {"main", "print"}});
        Sample s5({12.5, {}});

        std::vector<Sample> samples = {s1, s2, s3, s4, s5};
        std::vector<Event> events = convert_to_trace(samples);

        for (auto e : events) {
                e.print_debug();
        }

        return 0;
}
