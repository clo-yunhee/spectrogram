#include "appcontroller.h"

#include "../audio/audiofilereader.h"

AppController::AppController() {
    ui.setupUi(&window);

    connect(ui.actionOpen, &QAction::triggered, this, &AppController::handleOpenFile);

    connect(ui.actionSettings, &QAction::triggered, this, &AppController::openSettings);

    connect(ui.actionQuit, &QAction::triggered, this, &AppController::tryQuit);

    connect(&window, &AppWindow::tryQuit, this, &AppController::tryQuit);
}

AppController::~AppController() {}

void AppController::handleFileReadFailed(const QString& path, const QString& error) {
    QMessageBox::warning(&window, "Failed to read audio file",
                         QString("Failed to open audio file at %1\n%2").arg(path).arg(error));
}

void AppController::handleCurrentFileChanged(const QString& path, const QString& fileName) {
    window.setWindowTitle(QString("Spectrogram - %1").arg(fileName));
}

void AppController::show() { window.showNormal(); }

void AppController::close() { window.close(); }

void AppController::handleOpenFile() {
    QString extFilter = AudioFileReader::supportedFileFormats().join(", ");
    QString filePath = QFileDialog::getOpenFileName(
        &window, "Open audio file",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QString("Audio files (%1)").arg(extFilter));

    if (filePath.isNull()) {
        return;
    }

    emit tryOpenFile(filePath);
}