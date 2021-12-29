#include "maincontroller.h"

MainController::MainController() {
    settings.setWindowParent(app);

    connect(&audio, &AudioController::hostInfoRefreshed, &settings,
            &SettingsController::handleHostInfoRefreshed);

    connect(&audio, &AudioController::inputDeviceInfoRefreshed, &settings,
            &SettingsController::handleInputDeviceInfoRefreshed);

    connect(&audio, &AudioController::outputDeviceInfoRefreshed, &settings,
            &SettingsController::handleOutputDeviceInfoRefreshed);

    connect(&audio, &AudioController::fileReadSuccess, &track,
            &TrackController::handleFileReadSuccess);

    connect(&audio, &AudioController::fileReadFailed, &app, &AppController::handleFileReadFailed);

    connect(&track, &TrackController::currentFileChanged, &app,
            &AppController::handleCurrentFileChanged);

    connect(&app, &AppController::tryOpenFile, &audio, &AudioController::tryReadFile);

    connect(&app, &AppController::openSettings, &settings, &SettingsController::show);

    connect(&app, &AppController::tryQuit, this, &MainController::tryQuit);

    connect(&settings, &SettingsController::hostChanged, &audio,
            &AudioController::refreshDeviceInfo);

    connect(this, &MainController::started, &audio, &AudioController::refreshHostInfo);
}

MainController::~MainController() {}

void MainController::start() {
    app.show();

    emit started();
}

void MainController::tryQuit() {
    // TODO If track is modified, prompt confirmation
    qApp->quit();
}