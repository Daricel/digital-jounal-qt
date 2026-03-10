#ifndef JOURNALCONTROLLER_H
#define JOURNALCONTROLLER_H

#include "../repository/baserepo.h"
#include "../model/journalentry.h"
#include <memory>
#include <stack>
#include <vector>

#include "commands/command.h"
#include "filter/filter.h"

class JournalController {
public:
    explicit JournalController(std::unique_ptr<BaseRepository> repo);
    
    void addEntry(const QString &id,const QDate& date, const QString& title, const QString& content);
    void removeEntry(const QString& id);
    void updateEntry(const JournalEntry& entry);

    std::vector<JournalEntry> getAllEntries() const;
    JournalEntry getEntryById(const QString&id) const;
    std::vector<JournalEntry> getFilteredEntries(std::unique_ptr<Filter> filter) const;

    void undo();
    void redo();
    void clear();


private:
    std::unique_ptr<BaseRepository> m_repo;
    std::stack<std::unique_ptr<Command>> m_undoStack;
    std::stack<std::unique_ptr<Command>> m_redoStack;

};

#endif // JOURNALCONTROLLER_H