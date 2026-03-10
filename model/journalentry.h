#ifndef JOURNALENTRY_H
#define JOURNALENTRY_H

#include <QString>
#include <QDate>

class JournalEntry {
private:
    QString m_id;
    QDate m_date;
    QString m_title;
    QString m_content;

public:
    JournalEntry() = default;
    JournalEntry(const QString &id,const QDate& date, const QString& title, const QString& content);

    QString id() const;
    QDate date() const;
    QString title() const;
    QString  content() const;

    void setId(const QString& id);
    void setDate(const QDate& date);
    void setTitle(const QString& title);
    void setContent(const QString& content);

    friend QDataStream& operator<<(QDataStream& out, const JournalEntry& entry);
    friend QDataStream& operator>>(QDataStream& in, JournalEntry& entry);
    friend bool operator==(const JournalEntry& lhs, const JournalEntry& rhs);

};



#endif //JOURNALENTRY_H
