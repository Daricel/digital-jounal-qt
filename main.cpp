#include <QApplication>
#include "repository/jsonjournalrepo.h"
#include "ui/mainwindow.h"
#include "sample_entries.h"
#include "controller/controller.h"
#include "journal_test/journalunifiedtest.h"
#include "repository/csvjournalrepo.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    bool runTests = true;
    bool cleanRepository = false;

    for (int i = 1; i < argc; ++i) {
        QString arg = argv[i];
        if (arg == "--run-tests") {
            runTests = true;
        } else if (arg == "--clean") {
            cleanRepository = true;
        }
    }

    if (runTests) {
        qDebug() << "Running Journal Unified Tests...";
        int test_exit_code = runAllJournalTests(argc, argv);

        if (test_exit_code != 0) {
            qCritical() << "!!! WARNING: ONE OR MORE JOURNAL TESTS FAILED!!!";
        } else {
            qInfo() << "--- ALL JOURNAL UNIFIED TESTS PASSED SUCCESSFULLY! ---";
        }
        qDebug() << "Tests finished. Continuing with application startup...";
    } else {
        qDebug() << "To run tests, start with '--run-tests' argument.";
    }

    const bool USE_JSON_REPO = true;

    std::unique_ptr<BaseRepository> repo;

    if (USE_JSON_REPO) {
        repo = std::make_unique<JSONRepository>("journal_entries.json");
        qDebug() << "Using JSON repository";
    } else {
        repo = std::make_unique<CSVRepository>("journal_entries.csv");
        qDebug() << "Using CSV repository";
    }

    if (cleanRepository) {
        qDebug() << "Cleaning repository...";
        try {
            repo->clear();
            qInfo() << "Repository successfully cleaned";
        } catch (const std::exception& e) {
            qCritical() << "Failed to clean repository:" << e.what();
        }
    }

    JournalController controller(std::move(repo));
    if (!cleanRepository) {
        SampleEntries::populateRepository(&controller);
    }

    MainWindow w(controller);
    w.show();

    return a.exec();
}