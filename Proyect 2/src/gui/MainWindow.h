#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;
class QTextEdit;
class QTabWidget;
class QString;
struct MedLangAnalysisResult;

class MainWindow : public QMainWindow {
public:
    MainWindow();

private:
    void buildUi();
    void handleBrowse();
    void handleAnalyze();
    void handleExportReports();
    void loadSourcePreview(const QString& filePath);
    void renderAnalysis(const MedLangAnalysisResult& result);

    QLineEdit* m_filePathEdit;
    QPushButton* m_browseButton;
    QPushButton* m_analyzeButton;
    QPushButton* m_exportReportsButton;
    QLabel* m_statusLabel;
    QTabWidget* m_tabs;
    QTextEdit* m_sourceText;
    QTableWidget* m_tokensTable;
    QTableWidget* m_lexicalErrorsTable;
    QTableWidget* m_syntaxErrorsTable;
    QTableWidget* m_semanticErrorsTable;
};

#endif
