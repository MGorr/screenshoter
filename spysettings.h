#pragma once

#include <QString>

class SpySettings
{
public:
    SpySettings();

    int getMinutes() { return m_minutes; }
    int getSeconds() { return m_seconds; }

    int getMaxForMinutes();
    int getMaxForSeconds();

    QString getPathToSave() { return m_pathToSave; }

private:
    void setDefaultValues();

private:
    int m_minutes = 0;
    int m_seconds = 0;

    QString m_pathToSave;
};
