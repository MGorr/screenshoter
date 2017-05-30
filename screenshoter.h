#pragma once

#include "spysettings.h"

#include <QDir>
#include <QSystemTrayIcon>
#include <QWidget>

class QPushButton;
class QSpinBox;

class Screenshoter : public QWidget
{
    Q_OBJECT

public:
    Screenshoter(QWidget *parent = 0);
    ~Screenshoter();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void hide();
    void showNormal();
    void startShooting();
    void stopShooting();
    void shootScreen();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setAccessibility(bool value);
    void createActions();
    void createTrayIcon();

private:
    QPushButton *m_startBtn = nullptr;
    QPushButton *m_stopBtn = nullptr;
    QSpinBox *m_minutesSpin = nullptr;
    QSpinBox *m_secondsSpin = nullptr;
    QSystemTrayIcon *m_trayIcon = nullptr;
    QTimer *m_timer = nullptr;

    SpySettings m_settings;
    QDir m_saveDir;
};
