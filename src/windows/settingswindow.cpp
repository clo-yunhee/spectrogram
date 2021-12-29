#include "settingswindow.h"

#include "../dsp/windowfunctions.h"
#include "../utils.h"

Q_DECLARE_METATYPE(AudioHostInfo);
Q_DECLARE_METATYPE(AudioDeviceInfo);

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {
    ui.setupUi(this);

    setWindowFlag(Qt::Window);

    ui.freqResSuffixLabel->setFixedSize(ui.freqResSuffixLabel->sizeHint());
    ui.timeResSuffixLabel->setFixedSize(ui.timeResSuffixLabel->sizeHint());

    ui.windowComboBox->addItems(WindowFunctions::names());

    QObject::connect(ui.hostInterfaceComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
                     AudioDevices::instance(), &AudioDevices::refreshDeviceInfo);

    QObject::connect(ui.playbackDeviceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
                     &SettingsWindow::handleOutputDeviceSelected);

    QObject::connect(ui.recordingDeviceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
                     &SettingsWindow::handleInputDeviceSelected);

    QObject::connect(ui.sampleRateComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
                     &SettingsWindow::handleSampleRateSelected);

    QObject::connect(ui.freqResComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
                     &SettingsWindow::handleFrequencyResolutionChanged);

    QObject::connect(ui.timeResSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
                     &SettingsWindow::handleTimeResolutionChanged);

    loadSettings();

    adjustSize();
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::handleHostInfoRefreshed(const QVector<AudioHostInfo>& infos) {
    ui.hostInterfaceComboBox->clear();

    for (const auto& info : infos) {
        ui.hostInterfaceComboBox->addItem(info.name, QVariant::fromValue(info));
    }

    ui.hostInterfaceComboBox->setCurrentIndex(Pa_GetDefaultHostApi());
}

void SettingsWindow::handleInputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos) {
    ui.recordingDeviceComboBox->clear();

    const auto hostInfo = ui.hostInterfaceComboBox->currentData().value<AudioHostInfo>();

    int i = 0;

    for (const auto& info : infos) {
        ui.recordingDeviceComboBox->addItem(info.name, QVariant::fromValue(info));

        if (info.index == hostInfo.defaultInputDevice) {
            ui.recordingDeviceComboBox->setCurrentIndex(i);
        }

        ++i;
    }

    if (hostInfo.defaultInputDevice < 0) {
        ui.recordingDeviceComboBox->setCurrentIndex(0);
    }
}

void SettingsWindow::handleOutputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos) {
    ui.playbackDeviceComboBox->clear();

    const auto hostInfo = ui.hostInterfaceComboBox->currentData().value<AudioHostInfo>();

    int i = 0;

    for (const auto& info : infos) {
        ui.playbackDeviceComboBox->addItem(info.name, QVariant::fromValue(info));

        if (info.index == hostInfo.defaultOutputDevice) {
            ui.playbackDeviceComboBox->setCurrentIndex(i);
        }

        ++i;
    }

    if (hostInfo.defaultOutputDevice < 0) {
        ui.playbackDeviceComboBox->setCurrentIndex(0);
    }
}

void SettingsWindow::handleInputDeviceSelected(const int deviceIndex) {
    ui.sampleRateComboBox->clear();

    auto deviceInfo = ui.recordingDeviceComboBox->itemData(deviceIndex).value<AudioDeviceInfo>();

    int i = 0;

    if (!deviceInfo.availableSampleRates.contains(deviceInfo.defaultSampleRate)) {
        deviceInfo.availableSampleRates.append(deviceInfo.defaultSampleRate);
        std::sort(deviceInfo.availableSampleRates.begin(), deviceInfo.availableSampleRates.end());
    }

    for (const auto& sampleRate : deviceInfo.availableSampleRates) {
        ui.sampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), sampleRate);

        if (qFuzzyCompare(sampleRate, deviceInfo.defaultSampleRate)) {
            ui.sampleRateComboBox->setCurrentIndex(i);
        }

        ++i;
    }
}

void SettingsWindow::handleOutputDeviceSelected(const int deviceIndex) {
    auto deviceInfo = ui.playbackDeviceComboBox->itemData(deviceIndex).value<AudioDeviceInfo>();

    // TODO setting
}

void SettingsWindow::handleSampleRateSelected(const int index) {
    const double sampleRate = ui.sampleRateComboBox->itemData(index).toDouble();

    constexpr int nfft0 = 16;
    constexpr int nfft1 = 8192;

    ui.freqResComboBox->clear();

    for (int nfft = nfft0; nfft <= nfft1; nfft *= 2) {
        const double deltaF = sampleRate / nfft;

        ui.freqResComboBox->addItem(QString("%1 Hz").arg(deltaF, 0, 'f', 1), nfft);
    }

    updateResolutionLabels();
}

void SettingsWindow::handleFrequencyResolutionChanged(const int index) { updateResolutionLabels(); }

void SettingsWindow::handleTimeResolutionChanged(const double value) { updateResolutionLabels(); }

void SettingsWindow::loadSettings() {
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    // Make sure the config directory exists.
    QDir dir(configPath);
    if (!dir.exists()) dir.mkpath(".");

    m_settings = new QSettings(QString("%1/settings.ini").arg(configPath), QSettings::IniFormat, this);
}

void SettingsWindow::saveSettings() { m_settings->sync(); }

void SettingsWindow::updateResolutionLabels() {
    const double sampleRate = ui.sampleRateComboBox->currentData().toDouble();
    const int nfft = ui.freqResComboBox->currentData().toInt();
    const double updatesPerSec = ui.timeResSpinBox->value();

    // Update frequency resolution label.

    const QString freqText = qUnicode("(N\ua730\ua730\u1d1b = %1 points per transform)");

    ui.freqResSuffixLabel->setText(freqText.arg(nfft));

    // Update time resolution label.

    const QString label("updates/s (%1 ms, %2 samples)");

    // TODO: adjust the actual number so there's no overlap between windows.
    const int samples = nfft;
    const int dur = std::round(1000 * samples / sampleRate);

    ui.timeResSuffixLabel->setText(label.arg(dur).arg(samples));
}