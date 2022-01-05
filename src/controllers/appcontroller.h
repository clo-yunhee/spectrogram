#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QMainWindow>
#include <QObject>

#include "../windows/appwindow.h"
#include "settingscontroller.h"

namespace Ui {
class App;
}

class AppController : public QObject {
    Q_OBJECT
   public:
    AppController();
    virtual ~AppController();

   public slots:
    void handleFileReadFailed(const QString& path, const QString& error);
    void handleCurrentFileChanged(const QString& path, const QString& fileName);

    void handleSpectrogramUpdated(const Eigen::MatrixXd& matrix);

    void show();
    void close();

   private slots:
    void handleOpenFile();

   signals:
    void tryOpenFile(const QString& path);
    void openSettings();
    void tryQuit();

   private:
    AppWindow window;
    Ui::App* ui;

    friend void SettingsController::setWindowParent(AppController&);
};

#endif  // APPCONTROLLER_H