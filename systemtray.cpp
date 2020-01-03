#include "systemtray.h"

#include <QMenu>
#include <QSystemTrayIcon>

SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    QIcon libkiIcon = QIcon(":/images/libki_clock.png");

    QMenu *trayIconMenu = new QMenu();

    QAction *quitAction = new QAction(QIcon(":images/icons8-shutdown-90.png"), tr("Quit"), this);
    //connect(logoutAction, SIGNAL(triggered()), this, SLOT(doLogoutDialog()));
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon *trayIcon;
    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(libkiIcon);
    trayIcon->show();

}

