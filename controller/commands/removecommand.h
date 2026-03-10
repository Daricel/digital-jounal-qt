#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H

#include "command.h"
#include "../../repository/baserepo.h"
#include "../../model/journalentry.h"

class RemoveCommand : public Command {
public:
    RemoveCommand(BaseRepository* repo, const JournalEntry& entry);
    void execute() override;
    void undo() override;

private:
    BaseRepository* m_repo;
    JournalEntry m_removedEntry;
};

#endif // REMOVECOMMAND_H