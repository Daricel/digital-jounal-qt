#ifndef TITLEFILTER_H
#define TITLEFILTER_H

#include "filter.h"
#include <QString>


class TitleFilter : public Filter {
public:

    TitleFilter(const QString& keyword);

    std::vector<JournalEntry> apply(const std::vector<JournalEntry>& entries) const override;

private:
    QString m_keyword;
};

#endif // TITLEFILTER_H