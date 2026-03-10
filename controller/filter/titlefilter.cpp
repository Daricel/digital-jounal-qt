#include "titlefilter.h"
#include "../model/journalentry.h"
#include <algorithm>

TitleFilter::TitleFilter(const QString& keyword)
    : m_keyword(keyword) {}

std::vector<JournalEntry> TitleFilter::apply(const std::vector<JournalEntry>& entries) const {
    std::vector<JournalEntry> filteredEntries;
    if (m_keyword.isEmpty()) {
        return entries;
    }

    QString lowerKeyword = m_keyword.toLower();

    for (const auto& entry : entries) {
        if (entry.title().toLower().contains(lowerKeyword)) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}