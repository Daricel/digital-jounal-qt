#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../model/journalentry.h"

class EntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EntryDialog(QWidget *parent = nullptr, const JournalEntry* entry = nullptr);

    JournalEntry getEntry() const;
    bool isValid() const;

private slots:
    void validateInput();

private:
    QLineEdit *m_idEdit;
    QDateEdit *m_dateEdit;
    QLineEdit *m_titleEdit;
    QTextEdit *m_contentEdit;
    QPushButton *m_okButton;
    bool m_isEdit;
    QString m_originalId;
};

#endif // ENTRYDIALOG_H
