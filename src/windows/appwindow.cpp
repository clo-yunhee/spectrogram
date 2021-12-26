#include "appwindow.h"

AppWindow::AppWindow() : m_settings(this) {
    ui.setupUi(this);

    QObject::connect(ui.actionSettings, &QAction::triggered, &m_settings, &QWidget::showNormal);

    QObject::connect(&m_audioDevices, &AudioDevices::hostInfoRefreshed, &m_settings,
                     &SettingsWindow::handleHostInfoRefreshed);
    QObject::connect(&m_audioDevices, &AudioDevices::deviceInfoRefreshed, &m_settings,
                     &SettingsWindow::handleDeviceInfoRefreshed);

    m_audioDevices.refreshHostInfo();
}

AppWindow::~AppWindow() {}