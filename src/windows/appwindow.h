#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QtWidgets>

#include "../audio/audiodevices.h"
#include "../audio/audioguard.h"
#include "settingswindow.h"
#include "ui/ui_app.h"

class AppWindow : public QMainWindow {
   public:
    AppWindow();
    virtual ~AppWindow();

   private:
    Ui::App ui;

    AudioGuard m_audioGuard;
    AudioDevices m_audioDevices;

    SettingsWindow m_settings;
};

#endif  // APPWINDOW_H