#include "updatecommand.h"
#include "../../repository/baserepo.h"
#include "../../model/journalentry.h"
UpdateCommand::UpdateCommand(BaseRepository* repo, const JournalEntry& oldEntry, const JournalEntry& newEntry)
    : m_repo(repo), m_oldEntry(oldEntry), m_newEntry(newEntry)
{
    if (!m_repo) {
        throw std::runtime_error("UpdateCommand: Repository pointer cannot be null.");
    }

    if (m_oldEntry.id() != m_newEntry.id()) {
        throw std::runtime_error("UpdateCommand: Old and new entries must have the same ID for an update.");
    }
}

void UpdateCommand::execute() {
    if (!m_repo) {
        qWarning() << "UpdateCommand: Repository is null during execute(). Cannot update.";
        return;
    }
    try {
        m_repo->updateEntry(m_newEntry);
        qDebug() << "UpdateCommand: Executed update for entry ID:" << m_newEntry.id();
    } catch (const std::exception& e) {
        qWarning() << "UpdateCommand: Failed to execute update for ID" << m_newEntry.id() << ". Error:" << e.what();

        throw;
    }
}
void UpdateCommand::undo() {
    if (!m_repo) {
        qWarning() << "UpdateCommand: Repository is null during undo(). Cannot undo update.";
        return;
    }
    try {

        m_repo->updateEntry(m_oldEntry);
        qDebug() << "UpdateCommand: Undid update for entry ID:" << m_oldEntry.id();
    } catch (const std::exception& e) {
        qWarning() << "UpdateCommand: Failed to undo update for ID" << m_oldEntry.id() << ". Error:" << e.what();

    }
}