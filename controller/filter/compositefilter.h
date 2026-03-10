#ifndef COMPOSITEFILTER_H
#define COMPOSITEFILTER_H
#include "filter.h"
#include <vector>
#include <memory>
#include <QSet>

enum class FilterCombinationType {
    AND,
    OR
};

class CompositeFilter : public Filter {
public:

    explicit CompositeFilter(FilterCombinationType type, std::vector<std::unique_ptr<Filter>> filters);
    std::vector<JournalEntry> apply(const std::vector<JournalEntry>& entries) const override;

private:
    FilterCombinationType m_type;
    std::vector<std::unique_ptr<Filter>> m_filters;
};

#endif // COMPOSITEFILTER_H