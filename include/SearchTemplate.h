#ifndef SEARCH_TEMPLATE_H
#define SEARCH_TEMPLATE_H

#include <vector>
#include <memory>

// A generic search template function that searches a list of shared pointers using a custom predicate.
// T is the class type, Predicate is a callable (lambda, function pointer, or functor) returning bool.
template <typename T, typename Predicate>
std::vector<std::shared_ptr<T>> searchItems(const std::vector<std::shared_ptr<T>>& items, Predicate pred) {
    std::vector<std::shared_ptr<T>> results;
    for (const auto& item : items) {
        if (item && pred(item)) {
            results.push_back(item);
        }
    }
    return results;
}

#endif // SEARCH_TEMPLATE_H
