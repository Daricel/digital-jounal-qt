#include "mainwindow.h"
#include "entrydialog.h"
#include "filterdialog.h"
#include "removedialog.h"
#include "../repository/csvjournalrepo.h"
#include "../controller/controller.h"
#include "../model/journalentry.h"
#include "../controller/filter/titlefilter.h"
#include "../controller/filter/contentfilter.h"
#include "../controller/filter/compositefilter.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QHeaderView>
#include <QDateTime>
#include <QUuid>
#include <algorithm>
#include <QMessageBox>
#include <QDebug>
#include "app_styles.h"

MainWindow::MainWindow(JournalController& controller, QWidget *parent)
: QMainWindow(parent), m_isFiltered(false), m_controller(controller)
{
    setupUI();
    setupMenuBar();
    refreshEntryList();
    updateButtonStates();

    setWindowTitle("Journal Application");
    resize(900, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);

    auto *mainLayout = new QVBoxLayout(m_centralWidget);

    AppStyles::applyStyles(this);

    // Main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(m_mainSplitter);

    // Entry tree widget
    m_entriesTreeWidget = new QTreeWidget();
    m_entriesTreeWidget->setHeaderLabels(QStringList() << "Date" << "Title");
    m_entriesTreeWidget->setColumnWidth(0, 120);
    m_entriesTreeWidget->setAlternatingRowColors(true);
    m_entriesTreeWidget->setExpandsOnDoubleClick(false);
    m_mainSplitter->addWidget(m_entriesTreeWidget);

    // Buttons group box
    m_buttonsGroupBox = new QGroupBox("Actions");
    m_buttonsGroupBox->setMaximumWidth(200);
    m_buttonsGroupBox->setMinimumWidth(180);

    auto *buttonsLayout = new QVBoxLayout(m_buttonsGroupBox);

    m_addButton = new QPushButton("Add Entry");
    m_removeButton = new QPushButton("Remove Entry");
    m_updateButton = new QPushButton("Update Entry");

    buttonsLayout->addWidget(m_addButton);
    buttonsLayout->addWidget(m_removeButton);
    buttonsLayout->addWidget(m_updateButton);

    buttonsLayout->addWidget(new QLabel()); // Spacer

    m_undoButton = new QPushButton("Undo");
    m_redoButton = new QPushButton("Redo");

    buttonsLayout->addWidget(m_undoButton);
    buttonsLayout->addWidget(m_redoButton);

    buttonsLayout->addWidget(new QLabel()); // Spacer

    m_filterButton = new QPushButton("Filter Entries");
    m_clearFilterButton = new QPushButton("Clear Filter");
    m_clearRepoButton = new QPushButton("Clear Repository");
    buttonsLayout->addWidget(m_filterButton);
    buttonsLayout->addWidget(m_clearFilterButton);
    buttonsLayout->addWidget(m_clearRepoButton);
    buttonsLayout->addStretch();

    m_mainSplitter->addWidget(m_buttonsGroupBox);
    m_mainSplitter->setSizes({700, 200});

    // Status bar
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel);

    // Connections
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::addEntry);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::removeEntry);
    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateEntry);
    connect(m_undoButton, &QPushButton::clicked, this, &MainWindow::undoAction);
    connect(m_redoButton, &QPushButton::clicked, this, &MainWindow::redoAction);
    connect(m_filterButton, &QPushButton::clicked, this, &MainWindow::filterEntries);
    connect(m_clearFilterButton, &QPushButton::clicked, this, &MainWindow::clearFilter);
    connect(m_clearRepoButton, &QPushButton::clicked, this, &MainWindow::clearRepository);

    connect(m_entriesTreeWidget, &QTreeWidget::itemExpanded, this, &MainWindow::onItemExpanded);
    connect(m_entriesTreeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);

}

void MainWindow::setupMenuBar()
{
    auto *fileMenu = menuBar()->addMenu("&File");

    auto *exitAction = new QAction("&Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    auto *editMenu = menuBar()->addMenu("&Edit");

    auto *addAction = new QAction("&Add Entry", this);
    addAction->setShortcut(QKeySequence::New);
    connect(addAction, &QAction::triggered, this, &MainWindow::addEntry);
    editMenu->addAction(addAction);

    auto *undoAction = new QAction("&Undo", this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoAction);
    editMenu->addAction(undoAction);

    auto *redoAction = new QAction("&Redo", this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redoAction);
    editMenu->addAction(redoAction);

}

void MainWindow::addEntry()
{
    EntryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted && dialog.isValid()) {
        try {
            JournalEntry newEntryFromDialog = dialog.getEntry();
            QString entryId = newEntryFromDialog.id().trimmed();

            // If the user did not provide an ID (left the field empty), generate a UUID
            if (entryId.isEmpty()) {
                entryId = generateNewId();
                newEntryFromDialog.setId(entryId); // Set the generated ID in the entry object
            }

            // Check if the ID already exists before calling the controller
            std::vector<JournalEntry> allCurrentEntries = m_controller.getAllEntries();
            bool idExists = false;
            for (const auto& existingEntry : allCurrentEntries) {
                if (existingEntry.id() == entryId) {
                    idExists = true;
                    break;
                }
            }

            if (idExists) {
                QMessageBox::warning(this, "Error", QString("Failed to add entry: An entry with ID '%1' already exists. Please use a unique ID or leave it empty to auto-generate.").arg(entryId));
                return;
            }
            m_controller.addEntry(newEntryFromDialog.id(), newEntryFromDialog.date(), newEntryFromDialog.title(), newEntryFromDialog.content());

            refreshEntryList();
            updateButtonStates();
            m_statusLabel->setText("Entry added successfully.");
        } catch (const std::exception& e) {
            // Catch any other exception that might be thrown by the controller/repository
            QMessageBox::warning(this, "Error", QString("Failed to add entry: %1").arg(e.what()));
        }
    }
}

void MainWindow::removeEntry()
{

    std::vector<JournalEntry> entries = m_isFiltered ? m_currentEntries : m_controller.getAllEntries();

    if (entries.empty()) {
        QMessageBox::information(this, "Information", "No entries to remove.");
        return;
    }

    RemoveDialog dialog(entries, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString entryId = dialog.getSelectedEntryId();
        if (!entryId.isEmpty()) {
            try {
                m_controller.removeEntry(entryId);
                refreshEntryList();
                updateButtonStates();
                m_statusLabel->setText("Entry removed successfully");
            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", QString("Failed to remove entry: %1").arg(e.what()));
            }
        }
    }
}

void MainWindow::updateEntry()
{
    auto *currentItem = m_entriesTreeWidget->currentItem();
    if (!currentItem || currentItem->data(0, Qt::UserRole + 1).toString() != "entry") {
        QMessageBox::information(this, "Information", "Please select an entry to update.");
        return;
    }

    QString entryId = currentItem->data(0, Qt::UserRole).toString();
    try {
        JournalEntry entry = m_controller.getEntryById(entryId);
        EntryDialog dialog(this, &entry);

        if (dialog.exec() == QDialog::Accepted && dialog.isValid()) {
            auto updatedEntry = dialog.getEntry();
            updatedEntry.setId(entryId);
            m_controller.updateEntry(updatedEntry);

            refreshEntryList();
            updateButtonStates();
            m_statusLabel->setText("Entry updated successfully");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to update entry: %1").arg(e.what()));
    }
}

void MainWindow::undoAction()
{
    try {
        m_controller.undo();
        refreshEntryList();
        updateButtonStates();
        m_statusLabel->setText("Undo completed");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Undo failed: %1").arg(e.what()));
    }
}

void MainWindow::redoAction()
{
    try {
        m_controller.redo();
        refreshEntryList();
        updateButtonStates();
        m_statusLabel->setText("Redo completed");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Redo failed: %1").arg(e.what()));
    }
}

void MainWindow::filterEntries()
{
    FilterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        std::vector<std::unique_ptr<Filter>> activeFilters;

        // Check if title filter is active and has a pattern
        if (dialog.isTitleFilterActive() && !dialog.getTitlePattern().isEmpty()) {
            activeFilters.push_back(std::make_unique<TitleFilter>(dialog.getTitlePattern()));
        }

        // Check if content filter is active and has a pattern
        if (dialog.isContentFilterActive() && !dialog.getContentPattern().isEmpty()) {
            activeFilters.push_back(std::make_unique<ContentFilter>(dialog.getContentPattern()));
        }

        std::unique_ptr<Filter> finalFilter;

        if (activeFilters.empty()) {
            // No active filters, clear the existing filter
            clearFilter();
            m_statusLabel->setText("No filter pattern provided. Filter cleared.");
            return;
        } else if (activeFilters.size() == 1) {
            // Only one active filter, use it directly
            finalFilter = std::move(activeFilters[0]);
        } else {
            // Two or more active filters, create a CompositeFilter
            FilterCombinationType combineType = dialog.getCombinationType();
            finalFilter = std::make_unique<CompositeFilter>(combineType, std::move(activeFilters));
        }

        if (finalFilter) {
            m_currentEntries = m_controller.getFilteredEntries(std::move(finalFilter));
            m_isFiltered = true;
            m_statusLabel->setText(m_statusLabel->text() + QString(" (%1 entries)").arg(m_currentEntries.size()));
        } else {
            clearFilter();
            m_statusLabel->setText("Filter could not be applied.");
        }

        refreshEntryList(m_currentEntries);
        updateButtonStates();
    }
}

void MainWindow::clearFilter()
{
    m_isFiltered = false;
    m_currentEntries.clear();
    refreshEntryList();
    updateButtonStates();
    m_statusLabel->setText("Filter cleared");
}

void MainWindow::onItemExpanded(QTreeWidgetItem *item)
{
    if (item->data(0, Qt::UserRole + 1).toString() == "entry" && item->childCount() == 0) {
        QString entryId = item->data(0, Qt::UserRole).toString();
        try {
            JournalEntry entry = m_controller.getEntryById(entryId);
            expandEntryContent(item, entry);
            m_expandedItems[item] = true;
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", QString("Failed to load entry content: %1").arg(e.what()));
        }
    }
}

void MainWindow::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->data(0, Qt::UserRole + 1).toString() == "entry") {
        updateEntry();
    }
}

void MainWindow::refreshEntryList()
{
    std::vector<JournalEntry> entries = m_controller.getAllEntries();
    refreshEntryList(entries);
}

void MainWindow::refreshEntryList(const std::vector<JournalEntry>& entries)
{
    // Save expanded state
    std::set<QString> expandedEntries;
    for (int i = 0; i < m_entriesTreeWidget->topLevelItemCount(); ++i) {
        auto *item = m_entriesTreeWidget->topLevelItem(i);
        if (item->isExpanded()) {
            expandedEntries.insert(item->data(0, Qt::UserRole).toString());
        }
    }

    m_entriesTreeWidget->clear();
    m_expandedItems.clear();

    // Sort entries by date (newest first), then by title for consistent ordering
    auto sortedEntries = entries;
    std::sort(sortedEntries.begin(), sortedEntries.end(),
              [](const JournalEntry& a, const JournalEntry& b) {
                  if (a.date() != b.date()) {
                      return a.date() > b.date();
                  }
                  return a.title() < b.title();
              });

    for (const auto& entry : sortedEntries) {
        auto *item = createEntryItem(entry);
        m_entriesTreeWidget->addTopLevelItem(item);

        // Restore expanded state
        if (expandedEntries.count(entry.id())) {
            expandEntryContent(item, entry);
            item->setExpanded(true);
            m_expandedItems[item] = true;
        }
    }

    m_entriesTreeWidget->resizeColumnToContents(0);
}

void MainWindow::updateButtonStates()
{
    bool hasEntries = !m_controller.getAllEntries().empty();
    m_removeButton->setEnabled(hasEntries);
    m_updateButton->setEnabled(hasEntries);
    m_clearFilterButton->setEnabled(m_isFiltered);
}

QTreeWidgetItem* MainWindow::createEntryItem(const JournalEntry& entry)
{
    auto *item = new QTreeWidgetItem();
    item->setText(0, entry.date().toString("yyyy-MM-dd"));
    item->setText(1, entry.title());
    item->setData(0, Qt::UserRole, entry.id());
    item->setData(0, Qt::UserRole + 1, "entry");

    // Add expand indicator
    item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

    return item;
}

void MainWindow::expandEntryContent(QTreeWidgetItem* item, const JournalEntry& entry)
{
    if (item->childCount() > 0) {
        return; // Already expanded
    }

    auto *contentItem = new QTreeWidgetItem();
    contentItem->setText(1, entry.content());
    contentItem->setData(0, Qt::UserRole + 1, "content");

    // Style the content differently
    QFont font = contentItem->font(1);
    font.setItalic(true);
    contentItem->setFont(1, font);

    item->addChild(contentItem);
}

QString MainWindow::generateNewId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
void MainWindow::clearRepository()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Clear",
                                "Are you sure you want to delete ALL entries?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        try {
            m_controller.clear();
            refreshEntryList();
            m_statusLabel->setText("Repository cleared successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                                QString("Failed to clear repository: %1").arg(e.what()));
        }
    }
}