#ifndef CONTENTFILTER_H
#define CONTENTFILTER_H

#include "filter.h"
#include <QString>


class ContentFilter : public Filter {
public:
    ContentFilter(const QString& keyword);
    std::vector<JournalEntry> apply(const std::vector<JournalEntry>& entries) const override;

private:
    QString m_keyword;
};

#endif // CONTENTFILTER_H