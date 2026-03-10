#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H

#include "command.h"
#include "../../repository/baserepo.h"
#include "../../model/journalentry.h"

class UpdateCommand : public Command {
public:
    UpdateCommand(BaseRepository* repo, const JournalEntry& oldEntry, const JournalEntry& newEntry);
    void execute() override;
    void undo() override;

private:
    BaseRepository* m_repo;
    JournalEntry m_oldEntry;
    JournalEntry m_newEntry;
};

#endif // UPDATECOMMAND_H