#include "command.h"
#include "../../repository/baserepo.h"
#include "../../model/journalentry.h"
#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H


class AddCommand : public Command {
public:
    AddCommand(BaseRepository* repo, const JournalEntry& entry);
    void execute() override;
    void undo() override;

private:
    BaseRepository* m_repo;
    JournalEntry m_entry;
};


#endif //ADDCOMMAND_H
