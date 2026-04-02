#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;

class MainWindow : public QMainWindow {
public:
    MainWindow();

private:
    void buildUi();
    void handleBrowse();
    void handleAnalyze();
    void handleOpenReports();

    QLineEdit* m_filePathEdit;
    QPushButton* m_browseButton;
    QPushButton* m_analyzeButton;
    QPushButton* m_openReportsButton;
    QLabel* m_statusLabel;
    QTableWidget* m_tokensTable;
    QTableWidget* m_lexicalErrorsTable;
    QTableWidget* m_syntaxErrorsTable;
    QTableWidget* m_semanticErrorsTable;
};

#endif
