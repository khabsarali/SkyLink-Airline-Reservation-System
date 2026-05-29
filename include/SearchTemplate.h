#ifndef SEARCH_TEMPLATE_H
#define SEARCH_TEMPLATE_H

#include <algorithm>

// A generic template search utility that copies elements matching a predicate using STL iterators.
template <typename InputIt, typename OutputIt, typename Predicate>
OutputIt searchItems(InputIt first, InputIt last, OutputIt d_first, Predicate pred) {
    return std::copy_if(first, last, d_first, pred);
}

#endif // SEARCH_TEMPLATE_H

