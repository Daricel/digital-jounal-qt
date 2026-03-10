#include "journalentry.h"

JournalEntry::JournalEntry(const QString&id,const QDate& date, const QString& title, const QString& content)
    : m_id(id),m_date(date), m_title(title), m_content(content) {}

QString JournalEntry::id() const {
    return m_id;
}

QDate JournalEntry::date() const {
    return m_date;
}
QString JournalEntry::title() const {
    return m_title;
}
QString JournalEntry::content() const {
    return m_content;
}
void JournalEntry::setId(const QString& id) {
    m_id = id;
}
void JournalEntry::setTitle(const QString& title) {
    m_title = title;
}
void JournalEntry::setContent(const QString& content) {
    m_content = content;
}
void JournalEntry::setDate(const QDate& date) {
    m_date = date;
}
QDataStream& operator<<(QDataStream& out, const JournalEntry& entry)
{
    out << entry.id();
    out << entry.date().toString(Qt::ISODate);
    out << entry.title() << entry.content();
    return out;
}

QDataStream& operator>>(QDataStream& in, JournalEntry& entry) {
    QString id,dateStr, title, content;
    in >>id >> dateStr >> title >> content;
    entry.m_date = QDate::fromString(dateStr, Qt::ISODate);
    entry.m_title = title;
    entry.m_content = content;
    return in;
}
bool operator==(const JournalEntry& lhs, const JournalEntry& rhs) {
    return lhs.m_id == rhs.m_id &&
            lhs.m_date == rhs.m_date &&
           lhs.m_title == rhs.m_title &&
           lhs.m_content == rhs.m_content;
}
