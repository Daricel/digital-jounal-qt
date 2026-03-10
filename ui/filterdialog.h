#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../controller/filter/compositefilter.h"

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);

    QString getTitlePattern() const;
    QString getContentPattern() const;
    FilterCombinationType getCombinationType() const;

    bool isTitleFilterActive() const;
    bool isContentFilterActive() const;

private slots:
    void onTitleFilterToggled(bool checked);
    void onContentFilterToggled(bool checked);
    void updateCombineVisibility();

private:
    QCheckBox *m_titleFilterCheckBox;
    QCheckBox *m_contentFilterCheckBox;

    QLineEdit *m_titlePatternEdit;
    QLineEdit *m_contentPatternEdit;

    QComboBox *m_combineTypeCombo;
    QLabel *m_combineLabel;
};

#endif // FILTERDIALOG_H
