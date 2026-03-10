#include "compositefilter.h"
#include <algorithm>

CompositeFilter::CompositeFilter(FilterCombinationType type, std::vector<std::unique_ptr<Filter>> filters)
    : m_type(type), m_filters(std::move(filters)) {}

std::vector<JournalEntry> CompositeFilter::apply(const std::vector<JournalEntry>& entries) const {
    if (m_filters.empty()) {
        return entries;
    }

    if (m_type == FilterCombinationType::AND) {
        std::vector<JournalEntry> currentFilteredEntries = entries;
        for (const auto& filter : m_filters) {

            currentFilteredEntries = filter->apply(currentFilteredEntries);
        }
        return currentFilteredEntries;
    }
    else
        { // FilterCombinationType::OR
        QSet<QString> uniqueIds;
        std::vector<JournalEntry> resultEntries;

        for (const auto& filter : m_filters) {

            std::vector<JournalEntry> filteredByOne = filter->apply(entries);

            for (const auto& entry : filteredByOne) {
                if (!uniqueIds.contains(entry.id())) {
                    resultEntries.push_back(entry);
                    uniqueIds.insert(entry.id());
                }
            }
        }
        return resultEntries;
    }
}