#include "jsonjournalrepo.h"
#include <QDebug>

JSONRepository::JSONRepository(const QString& filename) 
    : m_filename(filename) {
    load();
}

void JSONRepository::addEntry(const JournalEntry& entry) {
    // we chech for dublicate id before adding
    for (const auto& e : m_entries) {
        if (e.id() == entry.id()) {
            qWarning() << "Entry with ID \"" << entry.id() << "\" already exists. Skipping add.";
            return;
        }
    }
    m_entries.push_back(entry);
    save();
}
void JSONRepository::removeEntry(const QString& id) {
    bool found = false;
    for (size_t i = 0; i < m_entries.size(); ) {
        if (m_entries[i].id() == id) {
            m_entries.erase(m_entries.begin() + i);
            found = true;
            break; // because ids are unique we can break after we find it
        } else {
            i++;
        }
    }

    if (!found) {
        qDebug() << "Warning: Entry with ID" << id << "not found.";
        return;
    }
    save();
}

void JSONRepository::updateEntry(const JournalEntry& entry) {
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

std::vector<JournalEntry> JSONRepository::getAllEntries() const {
    return m_entries;
}

JournalEntry JSONRepository::getEntryById(const QString& id) const {
    for (const auto& entry : m_entries) {
        if (entry.id() == id) {
            return entry;
        }
    }
    return JournalEntry("", QDate(), "", "");
}
void JSONRepository::load() {
    m_entries.clear();
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray entriesArray = doc.array();
            for (const QJsonValue& value : entriesArray) {
                if (value.isObject()) {
                    m_entries.push_back(jsonToEntry(value.toObject()));
                }
            }
        }
        file.close();
    }
}
void JSONRepository::save() {
    QJsonArray entriesArray;
    for (const auto& entry : m_entries) {
        entriesArray.append(entryToJson(entry));
    }

    QFile file(m_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(entriesArray).toJson());
        file.close();
    }
}

QJsonObject JSONRepository::entryToJson(const JournalEntry& entry) const {
    QJsonObject obj;
    obj["id"] = entry.id(); // Add ID to JSON
    obj["date"] = entry.date().toString(Qt::ISODate);
    obj["title"] = entry.title();
    obj["content"] = entry.content();
    return obj;
}

JournalEntry JSONRepository::jsonToEntry(const QJsonObject& json) const {
    QString id = json["id"].toString();
    QDate date = QDate::fromString(json["date"].toString(), Qt::ISODate);
    QString title = json["title"].toString();
    QString content = json["content"].toString();
    return JournalEntry(id, date, title, content);
}

void JSONRepository::clear() {
    m_entries.clear();
    save();
}
