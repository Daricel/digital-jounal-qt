#include "removecommand.h"
#include "../../repository/baserepo.h"
#include "../../model/journalentry.h"
RemoveCommand::RemoveCommand(BaseRepository* repo, const JournalEntry& entry)
    : m_repo(repo), m_removedEntry(entry) {
    if (!m_repo) {
        return;
    }
}

void RemoveCommand::execute(){
    if (!m_repo) return;
    if (m_removedEntry.id().isEmpty()) {
        qWarning() << "RemoveCommand: Cannot execute - entry ID is empty.";
        return;
    }
    try {
        m_repo->removeEntry(m_removedEntry.id());
        qDebug() << "RemoveCommand: Executed remove for ID:" << m_removedEntry.id();
    } catch (const std::exception& e) {
        qDebug() << "RemoveCommand: Error while removing entry in execute(): " << e.what();
        throw;
    }
}

void RemoveCommand::undo(){
    if (!m_repo) return;
    try {
        m_repo->addEntry(m_removedEntry);
        qDebug() << "RemoveCommand: Undone remove (re-added) for ID:" << m_removedEntry.id();
    } catch (const std::exception& e) {
        qWarning() << "RemoveCommand: Error while adding entry in undo(): " << e.what();

    }
}