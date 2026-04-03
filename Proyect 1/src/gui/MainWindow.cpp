#include "MainWindow.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include "../app/MedLangService.h"

namespace {
QString toQString(const std::string& text) {
    return QString::fromUtf8(text.c_str());
}
}

MainWindow::MainWindow()
    : m_filePathEdit(nullptr)
    , m_browseButton(nullptr)
    , m_analyzeButton(nullptr)
    , m_exportReportsButton(nullptr)
    , m_statusLabel(nullptr)
    , m_tabs(nullptr)
    , m_sourceText(nullptr)
    , m_tokensTable(nullptr)
    , m_lexicalErrorsTable(nullptr)
    , m_syntaxErrorsTable(nullptr)
    , m_semanticErrorsTable(nullptr) {
    buildUi();
}

void MainWindow::buildUi() {
    setWindowTitle("MedLang - Analizador y Reportes");
    resize(1200, 760);

    QWidget* central = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(central);

    QHBoxLayout* fileRow = new QHBoxLayout();
    QLabel* fileLabel = new QLabel("Archivo .med:", central);
    m_filePathEdit = new QLineEdit(central);
    m_filePathEdit->setPlaceholderText("Selecciona un archivo de entrada...");

    m_browseButton = new QPushButton("Buscar", central);
    m_analyzeButton = new QPushButton("Analizar", central);
    m_exportReportsButton = new QPushButton("Exportar y abrir HTML", central);

    fileRow->addWidget(fileLabel);
    fileRow->addWidget(m_filePathEdit, 1);
    fileRow->addWidget(m_browseButton);
    fileRow->addWidget(m_analyzeButton);
    fileRow->addWidget(m_exportReportsButton);

    m_statusLabel = new QLabel("Listo para analizar.", central);

    m_tabs = new QTabWidget(central);

    m_sourceText = new QTextEdit(m_tabs);
    m_sourceText->setReadOnly(true);
    m_sourceText->setPlaceholderText("Vista previa del archivo .med");

    m_tokensTable = new QTableWidget(0, 5, m_tabs);
    m_tokensTable->setHorizontalHeaderLabels({"#", "Tipo", "Lexema", "Linea", "Columna"});
    m_tokensTable->horizontalHeader()->setStretchLastSection(true);
    m_tokensTable->verticalHeader()->setVisible(false);
    m_tokensTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_lexicalErrorsTable = new QTableWidget(0, 6, m_tabs);
    m_lexicalErrorsTable->setHorizontalHeaderLabels({"#", "Lexema", "Tipo", "Descripcion", "Linea", "Columna"});
    m_lexicalErrorsTable->horizontalHeader()->setStretchLastSection(true);
    m_lexicalErrorsTable->verticalHeader()->setVisible(false);
    m_lexicalErrorsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_syntaxErrorsTable = new QTableWidget(0, 6, m_tabs);
    m_syntaxErrorsTable->setHorizontalHeaderLabels({"#", "Mensaje", "Esperado", "Encontrado", "Linea", "Columna"});
    m_syntaxErrorsTable->horizontalHeader()->setStretchLastSection(true);
    m_syntaxErrorsTable->verticalHeader()->setVisible(false);
    m_syntaxErrorsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_semanticErrorsTable = new QTableWidget(0, 5, m_tabs);
    m_semanticErrorsTable->setHorizontalHeaderLabels({"#", "Tipo", "Descripcion", "Entidad", "Linea"});
    m_semanticErrorsTable->horizontalHeader()->setStretchLastSection(true);
    m_semanticErrorsTable->verticalHeader()->setVisible(false);
    m_semanticErrorsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tabs->addTab(m_sourceText, "Entrada .med");
    m_tabs->addTab(m_tokensTable, "Tokens");
    m_tabs->addTab(m_lexicalErrorsTable, "Errores Lexicos");
    m_tabs->addTab(m_syntaxErrorsTable, "Errores Sintacticos");
    m_tabs->addTab(m_semanticErrorsTable, "Errores Semanticos");

    rootLayout->addLayout(fileRow);
    rootLayout->addWidget(m_statusLabel);
    rootLayout->addWidget(m_tabs, 1);

    setCentralWidget(central);

    connect(m_browseButton, &QPushButton::clicked, this, [this]() { handleBrowse(); });
    connect(m_analyzeButton, &QPushButton::clicked, this, [this]() { handleAnalyze(); });
    connect(m_exportReportsButton, &QPushButton::clicked, this, [this]() { handleExportReports(); });
}

void MainWindow::handleBrowse() {
    const QString selected = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo MedLang",
        QDir::currentPath(),
        "Archivos MedLang (*.med);;Todos los archivos (*.*)");

    if (!selected.isEmpty()) {
        m_filePathEdit->setText(selected);
        loadSourcePreview(selected);
    }
}

void MainWindow::handleAnalyze() {
    const QString filePath = m_filePathEdit->text().trimmed();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "MedLang", "Selecciona un archivo .med antes de analizar.");
        return;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        QMessageBox::warning(this, "MedLang", "La ruta seleccionada no es un archivo valido.");
        return;
    }

    loadSourcePreview(filePath);

    MedLangService service;
    const MedLangAnalysisResult result = service.analyzeFile(filePath.toStdString());

    renderAnalysis(result);
}

void MainWindow::handleExportReports() {
    const QString filePath = m_filePathEdit->text().trimmed();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "MedLang", "Selecciona un archivo .med antes de exportar reportes.");
        return;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        QMessageBox::warning(this, "MedLang", "La ruta seleccionada no es un archivo valido.");
        return;
    }

    loadSourcePreview(filePath);

    MedLangService service;
    const MedLangAnalysisResult result = service.analyzeFile(filePath.toStdString());

    renderAnalysis(result);

    if (result.htmlReportsGenerated) {
        const QString indexPath = QDir::current().filePath("output/indice_reportes.html");
        const QFileInfo info(indexPath);
        if (info.exists()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(indexPath));
            m_statusLabel->setText("Reportes HTML exportados y abiertos en el navegador.");
        } else {
            m_statusLabel->setText("Reportes HTML exportados, pero no se encontro output/indice_reportes.html.");
        }
    } else {
        QMessageBox::information(this, "MedLang", "No se pudieron exportar los reportes HTML.");
    }
}

void MainWindow::loadSourcePreview(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_sourceText->setPlainText("No se pudo cargar la vista previa del archivo.");
        return;
    }

    const QByteArray content = file.readAll();
    m_sourceText->setPlainText(QString::fromUtf8(content));
}

void MainWindow::renderAnalysis(const MedLangAnalysisResult& result) {

    if (!result.sourceLoaded) {
        m_statusLabel->setText("No se pudo abrir el archivo para analisis.");
        return;
    }

    // Reforzar ocultamiento del indice de filas de Qt para evitar duplicado con la columna "#".
    m_tokensTable->verticalHeader()->setVisible(false);
    m_lexicalErrorsTable->verticalHeader()->setVisible(false);
    m_syntaxErrorsTable->verticalHeader()->setVisible(false);
    m_semanticErrorsTable->verticalHeader()->setVisible(false);

    m_tokensTable->clearContents();
    m_tokensTable->setRowCount(0);
    for (std::size_t i = 0; i < result.tokens.size(); ++i) {
        const Token& token = result.tokens[i];
        const int row = m_tokensTable->rowCount();
        m_tokensTable->insertRow(row);
        m_tokensTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_tokensTable->setItem(row, 1, new QTableWidgetItem(toQString(tokenTypeToString(token.type))));
        m_tokensTable->setItem(row, 2, new QTableWidgetItem(toQString(token.lexeme)));
        m_tokensTable->setItem(row, 3, new QTableWidgetItem(QString::number(token.line)));
        m_tokensTable->setItem(row, 4, new QTableWidgetItem(QString::number(token.column)));
    }

    m_lexicalErrorsTable->clearContents();
    m_lexicalErrorsTable->setRowCount(0);
    for (std::size_t i = 0; i < result.lexicalErrors.size(); ++i) {
        const LexicalError& err = result.lexicalErrors[i];
        const int row = m_lexicalErrorsTable->rowCount();
        m_lexicalErrorsTable->insertRow(row);
        m_lexicalErrorsTable->setItem(row, 0, new QTableWidgetItem(QString::number(err.number)));
        m_lexicalErrorsTable->setItem(row, 1, new QTableWidgetItem(toQString(err.invalidLexeme)));
        m_lexicalErrorsTable->setItem(row, 2, new QTableWidgetItem(toQString(err.errorType)));
        m_lexicalErrorsTable->setItem(row, 3, new QTableWidgetItem(toQString(err.description)));
        m_lexicalErrorsTable->setItem(row, 4, new QTableWidgetItem(QString::number(err.line)));
        m_lexicalErrorsTable->setItem(row, 5, new QTableWidgetItem(QString::number(err.column)));
    }

    m_syntaxErrorsTable->clearContents();
    m_syntaxErrorsTable->setRowCount(0);
    for (std::size_t i = 0; i < result.syntaxErrors.size(); ++i) {
        const SyntaxError& err = result.syntaxErrors[i];
        const int row = m_syntaxErrorsTable->rowCount();
        m_syntaxErrorsTable->insertRow(row);
        m_syntaxErrorsTable->setItem(row, 0, new QTableWidgetItem(QString::number(err.number)));
        m_syntaxErrorsTable->setItem(row, 1, new QTableWidgetItem(toQString(err.message)));
        m_syntaxErrorsTable->setItem(row, 2, new QTableWidgetItem(toQString(err.expected)));
        m_syntaxErrorsTable->setItem(row, 3, new QTableWidgetItem(toQString(err.found)));
        m_syntaxErrorsTable->setItem(row, 4, new QTableWidgetItem(QString::number(err.line)));
        m_syntaxErrorsTable->setItem(row, 5, new QTableWidgetItem(QString::number(err.column)));
    }

    m_semanticErrorsTable->clearContents();
    m_semanticErrorsTable->setRowCount(0);
    for (std::size_t i = 0; i < result.semanticValidation.errors.size(); ++i) {
        const SemanticError& err = result.semanticValidation.errors[i];
        const int row = m_semanticErrorsTable->rowCount();
        m_semanticErrorsTable->insertRow(row);
        m_semanticErrorsTable->setItem(row, 0, new QTableWidgetItem(QString::number(err.number)));
        m_semanticErrorsTable->setItem(row, 1, new QTableWidgetItem(toQString(err.type)));
        m_semanticErrorsTable->setItem(row, 2, new QTableWidgetItem(toQString(err.description)));
        m_semanticErrorsTable->setItem(row, 3, new QTableWidgetItem(toQString(err.entity)));
        m_semanticErrorsTable->setItem(row, 4, new QTableWidgetItem(QString::number(err.line)));
    }

    const QString summary = QString("Analisis completado. Tokens: %1 | Lexicos: %2 | Sintacticos: %3 | Semanticos: %4")
        .arg(static_cast<int>(result.tokens.size()))
        .arg(static_cast<int>(result.lexicalErrors.size()))
        .arg(static_cast<int>(result.syntaxErrors.size()))
        .arg(static_cast<int>(result.semanticValidation.errors.size()));
    m_statusLabel->setText(summary);
}

