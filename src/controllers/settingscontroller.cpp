#include "settingscontroller.h"

#include "../dsp/windowfunctions.h"
#include "../utils.h"
#include "appcontroller.h"

SettingsController::SettingsController() {
    ui.setupUi(&window);
    ui.freqResSuffixLabel->setFixedSize(ui.freqResSuffixLabel->sizeHint());
    ui.timeResSuffixLabel->setFixedSize(ui.timeResSuffixLabel->sizeHint());
    ui.windowComboBox->addItems(WindowFunctions::names());
    window.adjustSize();

    connect(ui.hostInterfaceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsController::handleHostSelected);

    connect(ui.playbackDeviceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsController::handleOutputDeviceSelected);

    connect(ui.recordingDeviceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsController::handleInputDeviceSelected);

    connect(ui.sampleRateComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsController::handleSampleRateSelected);

    connect(ui.freqResComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsController::handleFrequencyResolutionChanged);

    connect(ui.timeResSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
            &SettingsController::handleTimeResolutionChanged);
}

SettingsController::~SettingsController() {}

void SettingsController::setWindowParent(AppController& app) {
    window.setParent(&app.window);
    window.setWindowFlag(Qt::Window);
}

void SettingsController::handleHostInfoRefreshed(const QVector<AudioHostInfo>& infos) {
    ui.hostInterfaceComboBox->clear();

    for (const auto& info : infos) {
        ui.hostInterfaceComboBox->addItem(info.name, QVariant::fromValue(info));
    }

    ui.hostInterfaceComboBox->setCurrentIndex(Pa_GetDefaultHostApi());
}

void SettingsController::handleInputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos) {
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

void SettingsController::handleOutputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos) {
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

void SettingsController::show() { window.showNormal(); }

void SettingsController::close() { window.close(); }

void SettingsController::load() {
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    // Make sure the config directory exists.
    QDir dir(configPath);
    if (!dir.exists()) dir.mkpath(".");

    settings = std::make_unique<QSettings>(QString("%1/settings.ini").arg(configPath),
                                           QSettings::IniFormat, &window);
}

void SettingsController::save() { settings->sync(); }

void SettingsController::handleHostSelected(const int hostApiIndex) {
    auto hostInfo = ui.hostInterfaceComboBox->itemData(hostApiIndex).value<AudioHostInfo>();

    // TODO settings

    emit hostChanged(hostApiIndex);
}

void SettingsController::handleInputDeviceSelected(const int deviceIndex) {
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

    emit inputDeviceChanged(deviceIndex);
}

void SettingsController::handleOutputDeviceSelected(const int deviceIndex) {
    auto deviceInfo = ui.playbackDeviceComboBox->itemData(deviceIndex).value<AudioDeviceInfo>();

    // TODO setting

    emit inputDeviceChanged(deviceIndex);
}

void SettingsController::handleSampleRateSelected(const int index) {
    const double sampleRate = ui.sampleRateComboBox->itemData(index).toDouble();

    constexpr int nfft0 = 16;
    constexpr int nfft1 = 8192;

    ui.freqResComboBox->clear();

    for (int nfft = nfft0; nfft <= nfft1; nfft *= 2) {
        const double deltaF = sampleRate / nfft;

        ui.freqResComboBox->addItem(QString("%1 Hz").arg(deltaF, 0, 'f', 1), nfft);
    }

    updateResolutionLabels();

    emit sampleRateChanged(sampleRate);
}

void SettingsController::handleFrequencyResolutionChanged(const int index) {
    updateResolutionLabels();

    const int nfft = ui.freqResComboBox->itemData(index).toInt();
    emit frequencyResolutionChanged(nfft);
}

void SettingsController::handleTimeResolutionChanged(const double value) {
    updateResolutionLabels();

    const double updatesPerSec = ui.timeResSpinBox->value();
    emit timeResolutionChanged(updatesPerSec);
}

void SettingsController::updateResolutionLabels() {
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
