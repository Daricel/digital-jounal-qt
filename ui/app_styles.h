#ifndef APP_STYLES_H
#define APP_STYLES_H

#include <QWidget>

class AppStyles {
public:
    static void applyStyles(QWidget* widget) {
        if (!widget) return;

        // Color Pallete
        // #F5F9FF - Very light blue (almost white) - for general background
        // #E6F0FA - Light blue - for groups and menus
        // #B8D4F0 - Medium light blue - for borders, group titles, selections
        // #2C7BE5 - Primary blue - for buttons, important actions
        // #1E6BC5 - Darker blue - for button hover, menu selections
        // #1552A0 - Even darker - for pressed buttons
        // #F0F5FA - Alternate rows in TreeWidget
        // #333333 - Dark gray for text
        // #FFFFFF - White for input backgrounds

        QString styleSheet =
            "QMainWindow { background-color: #F5F9FF; }"
            "QLabel { color: #333333; }"
            "QGroupBox { "
                "background-color: #E6F0FA; "
                "border: 1px solid #B8D4F0; "
                "border-radius: 8px; "
                "margin-top: 10px; "
            "}"
            "QGroupBox::title { "
                "subcontrol-origin: margin; "
                "subcontrol-position: top left; "
                "padding: 0 3px; "
                "background-color: #B8D4F0; "
                "color: #FFFFFF; "
                "border-radius: 5px; "
            "}"
            "QPushButton { "
                "background-color: #2C7BE5; "
                "color: white; "
                "border-radius: 8px; "
                "padding: 8px 15px; "
                "border: none; "
            "}"
            "QPushButton:hover { "
                "background-color: #1E6BC5; "
            "}"
            "QPushButton:pressed { "
                "background-color: #1552A0; "
            "}"
            "QTreeWidget { "
                "background-color: #FFFFFF; "
                "alternate-background-color: #F0F5FA; "
                "border: 1px solid #B8D4F0; "
                "border-radius: 8px; "
                "padding: 5px; "
            "}"
            "QTreeWidget::item { "
                "padding: 5px; "
                "color: #333333; "
            "}"
            "QTreeWidget::item:selected { "
                "background-color: #B8D4F0; "
                "color: #000000; "
            "}"
            "QMenuBar { background-color: #B8D4F0; color: white; }"
            "QMenuBar::item:selected { background-color: #1E6BC5; }"
            "QMenu { background-color: #E6F0FA; border: 1px solid #B8D4F0; }"
            "QMenu::item:selected { background-color: #1E6BC5; color: white; }"
            "QStatusBar { background-color: #B8D4F0; color: white; }"
            "QMessageBox { background-color: #F5F9FF; }"
            "QMessageBox QPushButton { background-color: #2C7BE5; color: white; border-radius: 5px; padding: 5px 10px; }"
            "QMessageBox QPushButton:hover { background-color: #1E6BC5; }"
            "QMessageBox QLabel { color: #333333; }"
            "QLineEdit, QTextEdit, QDateEdit, QComboBox { "
                "border: 1px solid #B8D4F0; "
                "border-radius: 5px; "
                "padding: 5px; "
                "background-color: #FFFFFF; "
                "color: #333333; "
                "selection-background-color: #B8D4F0; "
                "selection-color: white; "
            "}"
            "QComboBox::drop-down { border-left: 1px solid #B8D4F0; }"
            "QCheckBox::indicator { width: 15px; height: 15px; border: 1px solid #B8D4F0; border-radius: 3px; background-color: white; }"
            "QCheckBox::indicator:checked { background-color: #2C7BE5; }"
            ;

        widget->setStyleSheet(styleSheet);
    }
};

#endif // APP_STYLES_H