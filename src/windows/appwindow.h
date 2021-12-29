#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>

class AppWindow : public QMainWindow {
    Q_OBJECT
   public:
    AppWindow();
    virtual ~AppWindow();

   signals:
    void tryQuit();

   protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif  // APPWINDOW_H