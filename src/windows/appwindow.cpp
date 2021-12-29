#include "appwindow.h"

#include <QCloseEvent>

AppWindow::AppWindow() {}

AppWindow::~AppWindow() {}

void AppWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    emit tryQuit();
}
