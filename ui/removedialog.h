#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include "../model/journalentry.h"

class RemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDialog(const std::vector<JournalEntry>& entries, QWidget *parent = nullptr);

    QString getSelectedEntryId() const;

private:
    QListWidget *m_entriesListWidget;
    std::vector<JournalEntry> m_entries;
};

#endif // REMOVEDIALOG_H
