#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QGroupBox>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "entrydialog.h"
#include "filterdialog.h"
#include "removedialog.h"

#include "../controller/controller.h"
#include "../model/journalentry.h"
#include "../controller/filter/contentfilter.h"
#include "../controller/filter/titlefilter.h"
#include "../controller/filter/compositefilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(JournalController& controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addEntry();
    void removeEntry();
    void updateEntry();
    void undoAction();
    void redoAction();
    void filterEntries();
    void clearFilter();
    void onItemExpanded(QTreeWidgetItem *item);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void clearRepository();

private:
    void setupUI();
    void setupMenuBar();
    void refreshEntryList();
    void refreshEntryList(const std::vector<JournalEntry>& entries);
    void updateButtonStates();
    QTreeWidgetItem* createEntryItem(const JournalEntry& entry);
    void expandEntryContent(QTreeWidgetItem* item, const JournalEntry& entry);
    QString generateNewId() const;

    // UI Components
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QTreeWidget *m_entriesTreeWidget;
    QGroupBox *m_buttonsGroupBox;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_updateButton;
    QPushButton *m_undoButton;
    QPushButton *m_redoButton;
    QPushButton *m_filterButton;
    QPushButton *m_clearFilterButton;
    QLabel *m_statusLabel;
    QPushButton *m_clearRepoButton;

    // Controller
    JournalController& m_controller;

    // State
    bool m_isFiltered;
    std::vector<JournalEntry> m_currentEntries;
    std::map<QTreeWidgetItem*, bool> m_expandedItems;
};

#endif // MAINWINDOW_H
