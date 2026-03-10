#include "entrydialog.h"


EntryDialog::EntryDialog(QWidget *parent, const JournalEntry* entry)
    : QDialog(parent), m_isEdit(entry != nullptr)
{
    setWindowTitle(m_isEdit ? "Edit Entry" : "Add New Entry");
    setModal(true);
    resize(500, 400);

    auto *layout = new QVBoxLayout(this);

    // ID field
    layout->addWidget(new QLabel("ID:"));
    m_idEdit = new QLineEdit();
    m_idEdit->setEnabled(!m_isEdit); // ID is only editable for new entries
    layout->addWidget(m_idEdit);

    // Date field
    layout->addWidget(new QLabel("Date:"));
    m_dateEdit = new QDateEdit();
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDate(QDate::currentDate());
    layout->addWidget(m_dateEdit);

    // Title field
    layout->addWidget(new QLabel("Title:"));
    m_titleEdit = new QLineEdit();
    layout->addWidget(m_titleEdit);

    // Content field
    layout->addWidget(new QLabel("Content:"));
    m_contentEdit = new QTextEdit();
    layout->addWidget(m_contentEdit);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *cancelButton = new QPushButton("Cancel");
    m_okButton = new QPushButton(m_isEdit ? "Update" : "Add");
    m_okButton->setEnabled(false);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(m_okButton);
    layout->addLayout(buttonLayout);

    // Fill fields if editing
    if (entry) {
        m_originalId = entry->id();
        m_idEdit->setText(entry->id());
        m_dateEdit->setDate(entry->date());
        m_titleEdit->setText(entry->title());
        m_contentEdit->setText(entry->content());
    }

    // Connections
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_titleEdit, &QLineEdit::textChanged, this, &EntryDialog::validateInput);

    validateInput();
}

JournalEntry EntryDialog::getEntry() const
{
    return JournalEntry(
        m_idEdit->text().trimmed(),
        m_dateEdit->date(),
        m_titleEdit->text().trimmed(),
        m_contentEdit->toPlainText()
    );
}

bool EntryDialog::isValid() const
{
    return !m_titleEdit->text().trimmed().isEmpty();
}

void EntryDialog::validateInput()
{
    m_okButton->setEnabled(isValid());
}
