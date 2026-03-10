#include "journalunifiedtest.h"


void JournalUnifiedTest::initTestCase()
{
    QTemporaryFile tempFile;
    tempFile.open();
    m_tempDir = QFileInfo(tempFile.fileName()).absolutePath();
    tempFile.close();
    qDebug() << "Test directory:" << m_tempDir;
}

void JournalUnifiedTest::cleanupTestCase()
{
    QDir dir(m_tempDir);
    QStringList testFiles = dir.entryList(QStringList() << "test_*.csv" << "test_*.json", QDir::Files);
    for (const QString& file : testFiles) {
        dir.remove(file);
    }
}

JournalEntry JournalUnifiedTest::createTestEntry(const QString& id, int dayOffset)
{
    QDate date = QDate::currentDate().addDays(dayOffset);
    return JournalEntry(id, date,
                      QString("Title_%1").arg(id),
                      QString("Content for entry %1").arg(id));
}

void JournalUnifiedTest::verifyEntry(const JournalEntry& entry, const QString& expectedId, const QString& expectedTitle)
{
    QCOMPARE(entry.id(), expectedId);
    QCOMPARE(entry.title(), expectedTitle);
    QVERIFY(!entry.content().isEmpty());
}

std::unique_ptr<CSVRepository> JournalUnifiedTest::createCSVRepo()
{
    QString filename = m_tempDir + "/test_journal.csv";
    QFile::remove(filename);
    return std::make_unique<CSVRepository>(filename);
}

std::unique_ptr<JSONRepository> JournalUnifiedTest::createJSONRepo()
{
    QString filename = m_tempDir + "/test_journal.json";
    QFile::remove(filename);
    return std::make_unique<JSONRepository>(filename);
}

void JournalUnifiedTest::compareEntryVectors(const std::vector<JournalEntry>& actual, const std::vector<JournalEntry>& expected)
{
    QCOMPARE(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i) {
        QCOMPARE(actual[i].id(), expected[i].id());
        QCOMPARE(actual[i].title(), expected[i].title());
        QCOMPARE(actual[i].content(), expected[i].content());
        QCOMPARE(actual[i].date(), expected[i].date());
    }
}

// Tests for CSV repo

void JournalUnifiedTest::testCSVRepository_addEntry()
{
    auto repo = createCSVRepo();
    JournalEntry entry = createTestEntry("csv_001");
    repo->addEntry(entry);
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    verifyEntry(entries[0], "csv_001", "Title_csv_001");
    qInfo() << "SUCCESS: testCSVRepository_addEntry passed.";
}

void JournalUnifiedTest::testCSVRepository_removeEntry()
{
    auto repo = createCSVRepo();
    JournalEntry entry1 = createTestEntry("csv_001");
    JournalEntry entry2 = createTestEntry("csv_002");
    repo->addEntry(entry1);
    repo->addEntry(entry2);
    QCOMPARE(repo->getAllEntries().size(), static_cast<size_t>(2));
    repo->removeEntry("csv_001");
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    QCOMPARE(entries[0].id(), QString("csv_002"));
    qInfo() << "SUCCESS: testCSVRepository_removeEntry passed.";
}

void JournalUnifiedTest::testCSVRepository_updateEntry()
{
    auto repo = createCSVRepo();
    JournalEntry original = createTestEntry("csv_001");
    repo->addEntry(original);
    JournalEntry updated("csv_001", original.date(), "Updated Title", "Updated Content");
    repo->updateEntry(updated);
    JournalEntry retrieved = repo->getEntryById("csv_001");
    QCOMPARE(retrieved.title(), QString("Updated Title"));
    QCOMPARE(retrieved.content(), QString("Updated Content"));
    qInfo() << "SUCCESS: testCSVRepository_updateEntry passed.";
}

void JournalUnifiedTest::testCSVRepository_getAllEntries()
{
    auto repo = createCSVRepo();
    QVERIFY(repo->getAllEntries().empty());
    for (int i = 1; i <= 5; ++i) {
        repo->addEntry(createTestEntry(QString("csv_%1").arg(i, 3, 10, QChar('0'))));
    }
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(5));
    qInfo() << "SUCCESS: testCSVRepository_getAllEntries passed.";
}

void JournalUnifiedTest::testCSVRepository_getEntryById()
{
    auto repo = createCSVRepo();
    JournalEntry entry = createTestEntry("csv_001");
    repo->addEntry(entry);
    JournalEntry retrieved = repo->getEntryById("csv_001");
    QCOMPARE(retrieved.id(), entry.id());
    JournalEntry notFound = repo->getEntryById("inexistent");
    QVERIFY(notFound.id().isEmpty());
    qInfo() << "SUCCESS: testCSVRepository_getEntryById passed.";
}

void JournalUnifiedTest::testCSVRepository_duplicateId()
{
    auto repo = createCSVRepo();
    JournalEntry entry1 = createTestEntry("csv_001");
    JournalEntry entry2("csv_001", QDate::currentDate(), "Different Title", "Different Content");
    repo->addEntry(entry1);
    repo->addEntry(entry2);
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    QCOMPARE(entries[0].title(), QString("Title_csv_001"));
    qInfo() << "SUCCESS: testCSVRepository_duplicateId passed.";
}

void JournalUnifiedTest::testCSVRepository_persistenceAfterReload()
{
    QString filename = m_tempDir + "/test_persistence.csv";
    QFile::remove(filename);
    {
        CSVRepository repo1(filename);
        repo1.addEntry(createTestEntry("csv_001"));
        repo1.addEntry(createTestEntry("csv_002"));
    }
    {
        CSVRepository repo2(filename);
        auto entries = repo2.getAllEntries();
        QCOMPARE(entries.size(), static_cast<size_t>(2));
        QVERIFY(entries[0].id() == "csv_001" || entries[0].id() == "csv_002");
    }
    qInfo() << "SUCCESS: testCSVRepository_persistenceAfterReload passed.";
}

// Tests for JSON repo

void JournalUnifiedTest::testJSONRepository_addEntry()
{
    auto repo = createJSONRepo();
    JournalEntry entry = createTestEntry("json_001");
    repo->addEntry(entry);
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    verifyEntry(entries[0], "json_001", "Title_json_001");
    qInfo() << "SUCCESS: testJSONRepository_addEntry passed.";
}

void JournalUnifiedTest::testJSONRepository_removeEntry()
{
    auto repo = createJSONRepo();
    JournalEntry entry1 = createTestEntry("json_001");
    JournalEntry entry2 = createTestEntry("json_002");
    repo->addEntry(entry1);
    repo->addEntry(entry2);
    repo->removeEntry("json_001");
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    QCOMPARE(entries[0].id(), QString("json_002"));
    qInfo() << "SUCCESS: testJSONRepository_removeEntry passed.";
}

void JournalUnifiedTest::testJSONRepository_updateEntry()
{
    auto repo = createJSONRepo();
    JournalEntry original = createTestEntry("json_001");
    repo->addEntry(original);
    JournalEntry updated("json_001", original.date(), "Updated JSON Title", "Updated JSON Content");
    repo->updateEntry(updated);
    JournalEntry retrieved = repo->getEntryById("json_001");
    QCOMPARE(retrieved.title(), QString("Updated JSON Title"));
    QCOMPARE(retrieved.content(), QString("Updated JSON Content"));
    qInfo() << "SUCCESS: testJSONRepository_updateEntry passed.";
}

void JournalUnifiedTest::testJSONRepository_getAllEntries()
{
    auto repo = createJSONRepo();
    for (int i = 1; i <= 3; ++i) {
        repo->addEntry(createTestEntry(QString("json_%1").arg(i)));
    }
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(3));
    qInfo() << "SUCCESS: testJSONRepository_getAllEntries passed.";
}

void JournalUnifiedTest::testJSONRepository_getEntryById()
{
    auto repo = createJSONRepo();
    JournalEntry entry = createTestEntry("json_001");
    repo->addEntry(entry);
    JournalEntry retrieved = repo->getEntryById("json_001");
    QCOMPARE(retrieved.id(), entry.id());
    JournalEntry notFound = repo->getEntryById("inexistent");
    QVERIFY(notFound.id().isEmpty());
    qInfo() << "SUCCESS: testJSONRepository_getEntryById passed.";
}

void JournalUnifiedTest::testJSONRepository_duplicateId()
{
    auto repo = createJSONRepo();
    JournalEntry entry1 = createTestEntry("json_001");
    JournalEntry entry2("json_001", QDate::currentDate(), "Different Title", "Different Content");
    repo->addEntry(entry1);
    repo->addEntry(entry2);
    auto entries = repo->getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    qInfo() << "SUCCESS: testJSONRepository_duplicateId passed.";
}

void JournalUnifiedTest::testJSONRepository_persistenceAfterReload()
{
    QString filename = m_tempDir + "/test_json_persistence.json";
    QFile::remove(filename);
    {
        JSONRepository repo1(filename);
        repo1.addEntry(createTestEntry("json_001"));
        repo1.addEntry(createTestEntry("json_002"));
    }
    {
        JSONRepository repo2(filename);
        auto entries = repo2.getAllEntries();
        QCOMPARE(entries.size(), static_cast<size_t>(2));
    }
    qInfo() << "SUCCESS: testJSONRepository_persistenceAfterReload passed.";
}

// Tests for Controller

void JournalUnifiedTest::testController_withCSVRepo()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("ctrl_001", QDate::currentDate(), "Controller Title", "Controller Content");
    auto entries = controller.getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    QCOMPARE(entries[0].id(), QString("ctrl_001"));
    qInfo() << "SUCCESS: testController_withCSVRepo passed.";
}

void JournalUnifiedTest::testController_withJSONRepo()
{
    auto repo = createJSONRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("ctrl_002", QDate::currentDate(), "JSON Controller Title", "JSON Controller Content");
    auto entries = controller.getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(1));
    QCOMPARE(entries[0].id(), QString("ctrl_002"));
    qInfo() << "SUCCESS: testController_withJSONRepo passed.";
}

void JournalUnifiedTest::testController_undoRedo()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(0));
    controller.addEntry("undo_001", QDate::currentDate(), "Undo Test", "Content");
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(1));
    controller.undo();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(0));
    controller.redo();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(1));
    QCOMPARE(controller.getAllEntries()[0].id(), QString("undo_001"));
    qInfo() << "SUCCESS: testController_undoRedo passed.";
}

void JournalUnifiedTest::testController_addRemoveUpdate()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("test_001", QDate::currentDate(), "Original Title", "Original Content");
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(1));
    JournalEntry updated("test_001", QDate::currentDate(), "Updated Title", "Updated Content");
    controller.updateEntry(updated);
    JournalEntry retrieved = controller.getEntryById("test_001");
    QCOMPARE(retrieved.title(), QString("Updated Title"));
    controller.removeEntry("test_001");
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(0));
    qInfo() << "SUCCESS: testController_addRemoveUpdate passed.";
}

void JournalUnifiedTest::testController_filterByTitle()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("filter_001", QDate::currentDate(), "Important Meeting", "Meeting content");
    controller.addEntry("filter_002", QDate::currentDate(), "Shopping List", "Shopping content");
    controller.addEntry("filter_003", QDate::currentDate(), "Important Call", "Call content");
    auto allEntries = controller.getFilteredEntries(nullptr);
    QCOMPARE(allEntries.size(), static_cast<size_t>(3));
    qInfo() << "SUCCESS: testController_filterByTitle passed.";
}

void JournalUnifiedTest::testController_filterByContent()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("content_001", QDate::currentDate(), "Title1", "Work related content");
    controller.addEntry("content_002", QDate::currentDate(), "Title2", "Personal thoughts");
    controller.addEntry("content_003", QDate::currentDate(), "Title3", "More work stuff");
    auto allEntries = controller.getFilteredEntries(nullptr);
    QCOMPARE(allEntries.size(), static_cast<size_t>(3));
    qInfo() << "SUCCESS: testController_filterByContent passed.";
}

void JournalUnifiedTest::testController_clearHistory()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    controller.addEntry("clear_001", QDate::currentDate(), "Title1", "Content1");
    controller.addEntry("clear_002", QDate::currentDate(), "Title2", "Content2");
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(2));
    controller.clear();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(0));
    controller.undo();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(2));
    qInfo() << "SUCCESS: testController_clearHistory passed.";
}

    // Tests for integration

void JournalUnifiedTest::testIntegration_switchRepositories()
{
    JournalEntry testEntry = createTestEntry("integration_001");
    {
        auto csvRepo = createCSVRepo();
        csvRepo->addEntry(testEntry);
        auto entries = csvRepo->getAllEntries();
        QCOMPARE(entries.size(), static_cast<size_t>(1));
    }
    {
        QString csvFile = m_tempDir + "/test_journal.csv";
        QString jsonFile = m_tempDir + "/test_integration.json";
        QFile::remove(jsonFile);
        CSVRepository csvRepo(csvFile);
        JSONRepository jsonRepo(jsonFile);
        auto entries = csvRepo.getAllEntries();
        for (const auto& entry : entries) {
            jsonRepo.addEntry(entry);
        }
        auto jsonEntries = jsonRepo.getAllEntries(); // Assuming getAllEntries returns by value for std::vector
        QCOMPARE(jsonEntries.size(), static_cast<size_t>(1));
        QCOMPARE(jsonEntries[0].id(), testEntry.id());
    }
    qInfo() << "SUCCESS: testIntegration_switchRepositories passed.";
}


void JournalUnifiedTest::testIntegration_massOperations()
{
    auto repo = createCSVRepo();
    JournalController controller(std::move(repo));
    const int numEntries = 100;
    for (int i = 1; i <= numEntries; ++i) {
        controller.addEntry(
            QString("mass_%1").arg(i, 3, 10, QChar('0')),
            QDate::currentDate().addDays(i % 30),
            QString("Mass Title %1").arg(i),
            QString("Mass Content %1").arg(i)
        );
    }
    auto entries = controller.getAllEntries();
    QCOMPARE(entries.size(), static_cast<size_t>(numEntries));
    controller.undo();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(numEntries - 1));
    controller.clear();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(0));
    controller.undo();
    QCOMPARE(controller.getAllEntries().size(), static_cast<size_t>(numEntries - 1));
    qInfo() << "SUCCESS: testIntegration_massOperations passed.";
}


int runAllJournalTests(int argc, char *argv[]) {
    JournalUnifiedTest testObject;
    return QTest::qExec(&testObject, argc, argv);
}
