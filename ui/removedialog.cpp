#include "removedialog.h"


RemoveDialog::RemoveDialog(const std::vector<JournalEntry>& entries, QWidget *parent)
    : QDialog(parent), m_entries(entries)
{
    setWindowTitle("Select Entry to Remove");
    setModal(true);
    resize(500, 400);

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Select the entry you want to remove:"));

    m_entriesListWidget = new QListWidget();

    for (const auto& entry : entries) {
        auto *item = new QListWidgetItem();
        item->setText(QString("%1 - %2").arg(entry.date().toString("yyyy-MM-dd"), entry.title()));
        item->setData(Qt::UserRole, entry.id());
        m_entriesListWidget->addItem(item);
    }

    layout->addWidget(m_entriesListWidget);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *cancelButton = new QPushButton("Cancel");
    auto *removeButton = new QPushButton("Remove Selected");
    removeButton->setEnabled(false);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(removeButton);
    layout->addLayout(buttonLayout);

    // Connections
    connect(m_entriesListWidget, &QListWidget::itemSelectionChanged, [removeButton, this]() {
        removeButton->setEnabled(m_entriesListWidget->currentItem() != nullptr);
    });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(removeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_entriesListWidget, &QListWidget::itemDoubleClicked, this, &QDialog::accept);
}

QString RemoveDialog::getSelectedEntryId() const
{
    auto *currentItem = m_entriesListWidget->currentItem();
    return currentItem ? currentItem->data(Qt::UserRole).toString() : QString();
}
