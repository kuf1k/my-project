#ifndef DATAFILTER_H
#define DATAFILTER_H
#include <vector>
#include <functional>

namespace core {
    template<typename T>
    class DataFilter {
    public:
        static std::vector<T> filterBy(const std::vector<T>, std::function<bool(const T &)> predicate) {
            std::vector<T> result;
            for (const auto &item: items) {
                if (predicate(item)) {
                    result.push_back(item);
                }
            }
            return result;
        }

        static bool contains(const std::vector<T> &items, std::function<bool(const T &)> predicate) {
            return std::any_of(items.begin(), items.end(), predicate);
        }
    };
}

#endif //DATAFILTER_H
