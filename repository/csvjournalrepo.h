#ifndef CSVJOURNALREPO_H
#define CSVJOURNALREPO_H
#include "baserepo.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDate>
class CSVRepository : public BaseRepository {
public:
    explicit CSVRepository(const QString& filename);
    // CRUD operations
    void addEntry(const JournalEntry& entry) override;
    void removeEntry(const QString& id) override;
    void updateEntry(const JournalEntry& entry) override;
    // Query operations
    std::vector<JournalEntry> getAllEntries() const override;
    JournalEntry getEntryById(const QString& id) const override;
    // Persistence operations
    void load() override;
    void save() override;
    void clear() override;


private:
    QString m_filename;
    std::vector<JournalEntry> m_entries;
};


#endif //CSVJOURNALREPO_H
