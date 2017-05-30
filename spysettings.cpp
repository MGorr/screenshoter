#include "spysettings.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

namespace {
    const int MINUTES_DEFAULT = 1;
    const int SECONDS_DEFAULT = 0;
    const int MINUTES_MAXIMUM = 59;
    const int SECONDS_MAXIMUM = 60;

    const QString SETTINGS_FILE = "settings.ini";
    const QString GROUP_NAME = "SETTINGS";
    const QString PATH_DEFAULT = ".";
}

SpySettings::SpySettings()
{
    if (QFile(SETTINGS_FILE).exists()) {
        qDebug() << "Load settings from file.";
        QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
        settings.beginGroup(GROUP_NAME);
        m_minutes = settings.value("MINUTES", MINUTES_DEFAULT).toInt();
        if (m_minutes < 0 || m_minutes > MINUTES_MAXIMUM) {
            m_minutes = MINUTES_DEFAULT;
        }
        m_seconds = settings.value("SECONDS", SECONDS_DEFAULT).toInt();
        if (m_seconds < 0 || m_seconds > SECONDS_MAXIMUM) {
            m_seconds = SECONDS_DEFAULT;
        }
        m_pathToSave = settings.value("PATH", PATH_DEFAULT).toString();
        qDebug() << "Path to save:" << m_pathToSave;
    } else {
        qDebug() << "Load default settings.";
        setDefaultValues();
    }
}

void SpySettings::setDefaultValues()
{
    m_minutes = MINUTES_DEFAULT;
    m_seconds = SECONDS_DEFAULT;
    m_pathToSave = PATH_DEFAULT;
}

int SpySettings::getMaxForMinutes() { return MINUTES_MAXIMUM; }
int SpySettings::getMaxForSeconds() { return SECONDS_MAXIMUM; }
