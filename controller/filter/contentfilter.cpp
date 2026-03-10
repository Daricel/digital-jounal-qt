#include "contentfilter.h"
#include <algorithm>

ContentFilter::ContentFilter(const QString& keyword)
    : m_keyword(keyword) {}

std::vector<JournalEntry> ContentFilter::apply(const std::vector<JournalEntry>& entries) const {

    std::vector<JournalEntry> filteredEntries;

    if (m_keyword.isEmpty()) {
        return entries;
    }

    QString lowerKeyword = m_keyword.toLower();

    for (const auto& entry : entries) {
        if (entry.content().toLower().contains(lowerKeyword)) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}