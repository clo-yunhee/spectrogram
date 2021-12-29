#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "appcontroller.h"
#include "audiocontroller.h"
#include "settingscontroller.h"
#include "trackcontroller.h"

class MainController : public QObject {
    Q_OBJECT
   public:
    MainController();
    ~MainController();

    void start();

   public slots:
    void tryQuit();

   signals:
    void started();

   private:
    AudioController audio;
    TrackController track;

    AppController app;
    SettingsController settings;
};

#endif  // MAINCONTROLLER_H