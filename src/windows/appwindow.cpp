#include "appwindow.h"

AppWindow::AppWindow() : m_settings(this) {
    ui.setupUi(this);

    QObject::connect(ui.actionSettings, &QAction::triggered, &m_settings, &QWidget::showNormal);

    QObject::connect(&m_audioDevices, &AudioDevices::hostInfoRefreshed, &m_settings,
                     &SettingsWindow::handleHostInfoRefreshed);

    QObject::connect(&m_audioDevices, &AudioDevices::inputDeviceInfoRefreshed, &m_settings,
                     &SettingsWindow::handleInputDeviceInfoRefreshed);

    QObject::connect(&m_audioDevices, &AudioDevices::outputDeviceInfoRefreshed, &m_settings,
                     &SettingsWindow::handleOutputDeviceInfoRefreshed);

    m_audioDevices.refreshHostInfo();
}

AppWindow::~AppWindow() {}