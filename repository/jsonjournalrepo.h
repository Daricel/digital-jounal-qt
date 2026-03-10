#ifndef JSONREPOSITORY_H
#define JSONREPOSITORY_H

#include "baserepo.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

class JSONRepository : public BaseRepository {
public:
    explicit JSONRepository(const QString& filename);


    // CRUD operations
    void addEntry(const JournalEntry& entry) override;
    void removeEntry(const QString& id) override;
    void updateEntry(const JournalEntry& entry) override;

    std::vector<JournalEntry> getAllEntries() const override;
    JournalEntry getEntryById(const QString &id) const override;

    // Persistence operations
    void load() override;
    void save() override;
    void clear() override;


private:
    QString m_filename;
    std::vector<JournalEntry> m_entries;
    
    QJsonObject entryToJson(const JournalEntry& entry) const;
    JournalEntry jsonToEntry(const QJsonObject& json) const;
};

#endif // JSONREPOSITORY_H