#ifndef JOURNAL_UNIFIED_TEST_H
#define JOURNAL_UNIFIED_TEST_H

#include <QtTest/QtTest>
#include <QTemporaryFile>
#include <QDir>
#include <memory>
#include <QDebug> /
#include "csvjournalrepo.h"
#include "jsonjournalrepo.h"
#include "../repository/csvjournalrepo.h"
#include "../repository/jsonjournalrepo.h"
#include "../controller/controller.h"
#include "../model/journalentry.h"
#include "filter/filter.h"


class JournalUnifiedTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();


    void testCSVRepository_addEntry();
    void testCSVRepository_removeEntry();
    void testCSVRepository_updateEntry();
    void testCSVRepository_getAllEntries();
    void testCSVRepository_getEntryById();
    void testCSVRepository_duplicateId();
    void testCSVRepository_persistenceAfterReload();


    void testJSONRepository_addEntry();
    void testJSONRepository_removeEntry();
    void testJSONRepository_updateEntry();
    void testJSONRepository_getAllEntries();
    void testJSONRepository_getEntryById();
    void testJSONRepository_duplicateId();
    void testJSONRepository_persistenceAfterReload();


    void testController_withCSVRepo();
    void testController_withJSONRepo();
    void testController_undoRedo();
    void testController_addRemoveUpdate();
    void testController_filterByTitle();
    void testController_filterByContent();
    void testController_clearHistory();


    void testIntegration_switchRepositories();
    void testIntegration_massOperations();

private:
    QString m_tempDir;
    JournalEntry createTestEntry(const QString& id, int dayOffset = 0);
    void verifyEntry(const JournalEntry& entry, const QString& expectedId, const QString& expectedTitle);
    std::unique_ptr<CSVRepository> createCSVRepo();
    std::unique_ptr<JSONRepository> createJSONRepo();
    void compareEntryVectors(const std::vector<JournalEntry>& actual, const std::vector<JournalEntry>& expected);
};


int runAllJournalTests(int argc, char *argv[]);

#endif // JOURNAL_UNIFIED_TEST_H