#include "screenshoter.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>

Screenshoter::Screenshoter(QWidget *parent)
    : QWidget(parent), m_settings(), m_saveDir(m_settings.getPathToSave())
{
    // Design //
    auto layout = new QVBoxLayout;

    auto spinboxesLayout = new QHBoxLayout;
    layout->addLayout(spinboxesLayout);

    auto minLabel = new QLabel("Min:");
    spinboxesLayout->addWidget(minLabel);

    m_minutesSpin = new QSpinBox;
    m_minutesSpin->setValue(m_settings.getMinutes());
    m_minutesSpin->setMinimum(0);
    m_minutesSpin->setMaximum(m_settings.getMaxForMinutes());
    spinboxesLayout->addWidget(m_minutesSpin);

    auto secondsLabel = new QLabel("Sec:");
    spinboxesLayout->addWidget(secondsLabel);

    m_secondsSpin = new QSpinBox;
    m_secondsSpin->setValue(m_settings.getSeconds());
    m_secondsSpin->setMinimum(0);
    m_secondsSpin->setMaximum(m_settings.getMaxForSeconds());
    spinboxesLayout->addWidget(m_secondsSpin);

    m_startBtn = new QPushButton(tr("&Start"));
    connect(m_startBtn, SIGNAL(clicked()), this, SLOT(startShooting()));
    layout->addWidget(m_startBtn);

    m_stopBtn = new QPushButton(tr("Sto&p"));
    connect(m_stopBtn, SIGNAL(clicked()), this, SLOT(stopShooting()));
    layout->addWidget(m_stopBtn);

    setAccessibility(true);
    setLayout(layout);

    // Window //
    setWindowTitle(tr("Spy"));
    setFixedSize(185, 185);
    QIcon spyIcon(":/spy_64.png");
    setWindowIcon(spyIcon);

    // Tray //
    auto restoreAct = new QAction(tr("&Restore"), this);
    connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));

    auto quitAct = new QAction(tr("&Quit"), this);
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    auto trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAct);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAct);

    m_trayIcon = new QSystemTrayIcon(spyIcon, this);
    m_trayIcon->setToolTip(tr("Spy"));
    m_trayIcon->setContextMenu(trayIconMenu);

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    // Timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(shootScreen()));
}

Screenshoter::~Screenshoter() { }

void Screenshoter::closeEvent(QCloseEvent *event)
{
    if (m_timer->isActive()) {
        hide();
        event->ignore();
    } else {
        QWidget::closeEvent(event);
    }
}

void Screenshoter::hide()
{
    m_trayIcon->show();
    QWidget::hide();
}

void Screenshoter::showNormal()
{
    m_trayIcon->hide();
    QWidget::showNormal();
}

void Screenshoter::startShooting()
{
    if (m_minutesSpin->value() || m_secondsSpin->value()) {
        qDebug() << "Start shooting";
        setAccessibility(false);
        m_timer->start(1000 * (60 * m_minutesSpin->value() + m_secondsSpin->value()));
        hide();
        m_trayIcon->showMessage(tr("Attention!"), tr("I am watching you!"), QSystemTrayIcon::Information, 5000);
    }
}

void Screenshoter::stopShooting()
{
    qDebug() << "Stop shooting";
    m_timer->stop();
    setAccessibility(true);
}

void Screenshoter::shootScreen()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString subdirName = dateTime.toString("yyyy_MM_dd");
    bool result = m_saveDir.mkpath(subdirName);
    if (!result) {
        qDebug() << "Failed to use directory:" << m_saveDir;
        return;
    }
    QString screenName = dateTime.toString("/hh_mm_ss_zzz_%1.png");
    QDesktopWidget *dw = QApplication::desktop();
    QScreen *screen = QApplication::primaryScreen();
    int screenCount = dw->screenCount();
    for (int i=0; i < screenCount; ++i) {
        QPixmap pixmap = screen->grabWindow(dw->winId(), i*dw->width()/screenCount, 0, dw->width()/screenCount, dw->height());
        QString fileName = m_saveDir.path() + "/" + subdirName + screenName.arg(i);
        bool result = pixmap.save(fileName, "png");
        qDebug() << "File" << fileName << ":" << (result ? "OK" : "KO");
    }
}

void Screenshoter::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    default:
        break;
    case QSystemTrayIcon::DoubleClick:
        showNormal();
    }
}

void Screenshoter::setAccessibility(bool value)
{
    m_minutesSpin->setEnabled(value);
    m_secondsSpin->setEnabled(value);
    m_startBtn->setEnabled(value);
    m_stopBtn->setEnabled(!value);
}
