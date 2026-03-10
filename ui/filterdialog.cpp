#include "filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Filter Entries");
    setModal(true);
    resize(400, 300);

    auto *layout = new QVBoxLayout(this);


    m_titleFilterCheckBox = new QCheckBox("Filter by Title");
    layout->addWidget(m_titleFilterCheckBox);
    m_titlePatternEdit = new QLineEdit();
    m_titlePatternEdit->setPlaceholderText("e.g., *project* or notes*");
    m_titlePatternEdit->setEnabled(false);
    layout->addWidget(m_titlePatternEdit);

    // Content filter
    m_contentFilterCheckBox = new QCheckBox("Filter by Content");
    layout->addWidget(m_contentFilterCheckBox);
    m_contentPatternEdit = new QLineEdit();
    m_contentPatternEdit->setPlaceholderText("e.g., *meeting* or work*");
    m_contentPatternEdit->setEnabled(false);
    layout->addWidget(m_contentPatternEdit);

    // The type of the combination (AND/OR)
    m_combineLabel = new QLabel("Combine filters with:");
    layout->addWidget(m_combineLabel);
    m_combineTypeCombo = new QComboBox();
    m_combineTypeCombo->addItem("AND", static_cast<int>(FilterCombinationType::AND));
    m_combineTypeCombo->addItem("OR", static_cast<int>(FilterCombinationType::OR));
    layout->addWidget(m_combineTypeCombo);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    auto *cancelButton = new QPushButton("Cancel");
    auto *applyButton = new QPushButton("Apply Filter");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
    layout->addLayout(buttonLayout);

    // Conexions
    connect(m_titleFilterCheckBox, &QCheckBox::toggled, this, &FilterDialog::onTitleFilterToggled);
    connect(m_contentFilterCheckBox, &QCheckBox::toggled, this, &FilterDialog::onContentFilterToggled);
    connect(m_titleFilterCheckBox, &QCheckBox::toggled, this, &FilterDialog::updateCombineVisibility);
    connect(m_contentFilterCheckBox, &QCheckBox::toggled, this, &FilterDialog::updateCombineVisibility);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyButton, &QPushButton::clicked, this, &QDialog::accept);

    // initialize visibility
    updateCombineVisibility();
}

QString FilterDialog::getTitlePattern() const
{
    return m_titlePatternEdit->text().trimmed();
}

QString FilterDialog::getContentPattern() const
{
    return m_contentPatternEdit->text().trimmed();
}

FilterCombinationType FilterDialog::getCombinationType() const
{
    return static_cast<FilterCombinationType>(m_combineTypeCombo->currentData().toInt());
}

bool FilterDialog::isTitleFilterActive() const
{
    return m_titleFilterCheckBox->isChecked();
}

bool FilterDialog::isContentFilterActive() const
{
    return m_contentFilterCheckBox->isChecked();
}

void FilterDialog::onTitleFilterToggled(bool checked)
{
    m_titlePatternEdit->setEnabled(checked);
    if (!checked) {
        m_titlePatternEdit->clear();
    }
}

void FilterDialog::onContentFilterToggled(bool checked)
{
    m_contentPatternEdit->setEnabled(checked);
    if (!checked) {
        m_contentPatternEdit->clear();
    }
}

void FilterDialog::updateCombineVisibility()
{
    bool showCombine = m_titleFilterCheckBox->isChecked() && m_contentFilterCheckBox->isChecked();
    m_combineLabel->setVisible(showCombine);
    m_combineTypeCombo->setVisible(showCombine);
}
