#include "controller.h"
#include <QDebug>
#include <stdexcept>
#include <functional>
#include "commands/addcommand.h"
#include "commands/removecommand.h"
#include "commands/updatecommand.h"

namespace {
    class LambdaCommand : public Command {
    public:
        LambdaCommand(std::function<void()> executeFunc, std::function<void()> undoFunc)
            : m_executeFunc(executeFunc), m_undoFunc(undoFunc) {}

        void execute() override { m_executeFunc(); }
        void undo() override { m_undoFunc(); }

    private:
        std::function<void()> m_executeFunc;
        std::function<void()> m_undoFunc;
    };
}

JournalController::JournalController(std::unique_ptr<BaseRepository> repo)
    : m_repo(std::move(repo))
{
}

void JournalController::addEntry(const QString& id, const QDate& date, const QString& title, const QString& content) {
    JournalEntry newEntry(id, date, title, content);
    auto command = std::make_unique<AddCommand>(m_repo.get(), newEntry);

    try {
        command->execute();
    } catch (const std::exception& e) {
        qWarning() << "Controller: Failed to add entry. Error:" << e.what();
        return;
    }

    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
    m_undoStack.push(std::move(command));
    qDebug() << "Controller: Entry added. Undo stack size:" << m_undoStack.size();
}

void JournalController::removeEntry(const QString& id) {
    JournalEntry removedEntry;
    try {
        removedEntry = m_repo->getEntryById(id);
    } catch (const std::exception& e) {
        qWarning() << "Controller: Cannot remove entry. ID not found or error:" << e.what();
        return;
    }

    auto command = std::make_unique<RemoveCommand>(m_repo.get(), removedEntry);

    try {
        command->execute();
    } catch (const std::exception& e) {
        qWarning() << "Controller: Failed to remove entry. Error:" << e.what();
        return;
    }

    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }

    m_undoStack.push(std::move(command));

    qDebug() << "Controller: Entry removed. Undo stack size:" << m_undoStack.size();
}

void JournalController::updateEntry(const JournalEntry& entry) {
    JournalEntry oldEntry;
    try {
        oldEntry = m_repo->getEntryById(entry.id());
    } catch (const std::exception& e) {
        qWarning() << "Controller: Cannot update entry. Original ID not found or error:" << e.what();
        return;
    }

    auto command = std::make_unique<UpdateCommand>(m_repo.get(), oldEntry, entry);

    try {
        command->execute();
    } catch (const std::exception& e) {
        qWarning() << "Controller: Failed to update entry. Error:" << e.what();
        return;
    }
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }

    m_undoStack.push(std::move(command));

    qDebug() << "Controller: Entry updated. Undo stack size:" << m_undoStack.size();
}

std::vector<JournalEntry> JournalController::getAllEntries() const {
    return m_repo->getAllEntries();
}

JournalEntry JournalController::getEntryById(const QString& id) const {
    return m_repo->getEntryById(id);
}

std::vector<JournalEntry> JournalController::getFilteredEntries(std::unique_ptr<Filter> filter) const {
    std::vector<JournalEntry> allEntries = m_repo->getAllEntries();
    if (filter) {
        return filter->apply(allEntries);
    }
    qDebug() << "Controller: getFilteredEntries received a null filter  all the entries are returned";
    return allEntries;
}

void JournalController::undo() {
    if (!m_undoStack.empty()) {
        auto commandToUndo = std::move(m_undoStack.top());
        m_undoStack.pop();

        commandToUndo->undo();

        m_redoStack.push(std::move(commandToUndo));

        qDebug() << "Controller: Undo performed. Undo stack size:" << m_undoStack.size() << ", Redo stack size:" << m_redoStack.size();
    } else {
        qDebug() << "Controller: Cannot undo, undo stack is empty.";
    }
}

void JournalController::redo() {
    if (!m_redoStack.empty()) {
        auto commandToRedo = std::move(m_redoStack.top()); // Preia elementul de sus
        m_redoStack.pop();

        commandToRedo->execute();

        m_undoStack.push(std::move(commandToRedo));

        qDebug() << "Controller: Redo performed. Undo stack size:" << m_undoStack.size() << ", Redo stack size:" << m_redoStack.size();
    } else {
        qDebug() << "Controller: Cannot redo, redo stack is empty.";
    }
}

void JournalController::clear() {
    std::vector<JournalEntry> entriesToRestore = m_repo->getAllEntries();
    BaseRepository* rawRepoPtr = m_repo.get();

    auto command = std::make_unique<LambdaCommand>(
        [rawRepoPtr]() {
            try {
                rawRepoPtr->clear();
                qDebug() << "Clear command: Executed clear (all entries removed).";
            } catch (const std::exception& e) {
                qDebug() << "Clear command: Error while executing clear(): " << e.what();
                throw;
            }
        },
        [rawRepoPtr, entriesToRestore]() {
            try {
                rawRepoPtr->clear();
                for (const auto& entry : entriesToRestore) {
                    rawRepoPtr->addEntry(entry);
                }
                qDebug() << "Clear command: Undone clear (all entries re-added).";
            } catch (const std::exception& e) {
                qWarning() << "Clear command: Error while re-adding entries in undo(): " << e.what();
            }
        }
    );

    try {
        command->execute();
    } catch (const std::exception& e) {
        qWarning() << "Controller: Failed to clear entries. Error:" << e.what();
        return;
    }
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
    m_undoStack.push(std::move(command));
    qDebug() << "Controller: All entries cleared. Undo stack size:" << m_undoStack.size();
}