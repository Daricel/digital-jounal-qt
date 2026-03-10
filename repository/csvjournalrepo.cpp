#include "csvjournalrepo.h"
#include <QDebug>

CSVRepository::CSVRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

void CSVRepository::addEntry(const JournalEntry& entry) {
    for (const auto& e : m_entries) {
        if (e.id() == entry.id()) {
            qWarning() << "Entry with ID \"" << entry.id() << "\" already exists. Skipping add.";
            return;
        }
    }
    m_entries.push_back(entry);

    save();
}

void CSVRepository::removeEntry(const QString& id) {
    bool found = false;
    for (size_t i = 0; i < m_entries.size(); ) {
        if (m_entries[i].id() == id) {
            m_entries.erase(m_entries.begin() + i);
            found = true;
        } else {
            i++;
        }
    }
    if (!found) {
        qDebug() << "Warning: Entry not found for update.";
        return;
    }

    save();
}

void CSVRepository::updateEntry(const JournalEntry& entry) {
    bool found = false;
    for (auto& e : m_entries) {
        if (e.id() == entry.id()) {
            e = entry;
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "Warning: Entry with ID" << entry.id() << "not found for update.";
        return;
    }
    save();
}

std::vector<JournalEntry> CSVRepository::getAllEntries() const {
    return m_entries;
}

JournalEntry CSVRepository::getEntryById(const QString& id ) const {

    for (const auto& entry : m_entries) {
        if (entry.id() == id) {
            return entry;
        }
    }
    return JournalEntry();
}

void CSVRepository::load() {
    m_entries.clear();
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 4) {
                QString id = parts[0];
                QDate date = QDate::fromString(parts[1], Qt::ISODate);
                QString title = parts[2];
                QString content = parts[3];
                m_entries.emplace_back(id,date, title, content);
            }
        }
        file.close();
    }
}

void CSVRepository::save() {
    QFile file(m_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& entry : m_entries) {
            out <<entry.id() << ","
                <<entry.date().toString(Qt::ISODate) << ","
                << entry.title() << ","
                << entry.content() << "\n";
        }
        file.close();
    }
}
void CSVRepository::clear() {
    m_entries.clear();
    save();
}
