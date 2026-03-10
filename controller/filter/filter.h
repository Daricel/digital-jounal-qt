#ifndef FILTER_H
#define FILTER_H

#include "../../model/journalentry.h"
#include <vector>

class Filter {
public:
    virtual ~Filter() = default;
    virtual std::vector<JournalEntry> apply(const std::vector<JournalEntry>& entries) const = 0;
};

#endif // FILTER_H