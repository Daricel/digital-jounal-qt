#ifndef BASEREPO_H
#define BASEREPO_H
#include "../model/journalentry.h"
#include <vector>
#include <QDate>
class BaseRepository {
    public:
    virtual ~BaseRepository() = default;
    // CRUD operations
    virtual void addEntry(const JournalEntry& entry) = 0;
    virtual void removeEntry(const QString& id) = 0;
    virtual void updateEntry(const JournalEntry& entry) = 0;
    // Query operations
    virtual std::vector<JournalEntry> getAllEntries() const = 0;
    virtual JournalEntry getEntryById(const QString& id) const = 0;
    // Persistence operations
    virtual void load() = 0;
    virtual void save() = 0;
    virtual void clear() = 0;

};
#endif //BASEREPO_H
