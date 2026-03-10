#include "addcommand.h"
#include "../../repository/baserepo.h"
AddCommand::AddCommand(BaseRepository* repo, const JournalEntry& entry)
    : m_repo(repo), m_entry(entry) {
    if (!m_repo) {
        throw std::runtime_error("Repository pointer is null. Operation cannot continue.");
    }
}
void AddCommand::execute() {
    if (!m_repo) return;
    try {
        m_repo->addEntry(m_entry);
        qDebug() << "AddCommand: Executed add for ID:" << m_entry.id();
    } catch (const std::exception& e) {
        qDebug() << "AddCommand: Error while adding entry in execute(): " << e.what();
        throw;
    }
}

void AddCommand::undo() {
    if (!m_repo) return;
    if (m_entry.id().isEmpty()) {
        qWarning() << "AddCommand: Cannot undo - entry ID is empty.";
        return;
    }
    try {
        m_repo->removeEntry(m_entry.id());
        qDebug() << "AddCommand: Undone add (removed) for ID:" << m_entry.id();
    } catch (const std::exception& e) {
        qWarning() << "AddCommand: Error while removing entry in undo(): " << e.what();

    }
}