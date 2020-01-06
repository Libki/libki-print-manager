#include "systemtray.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    QIcon libkiIcon = QIcon(":/images/libki_clock.png");

    QMenu *trayIconMenu = new QMenu();

    QAction *aboutAction = new QAction(QIcon(":images/icons8-about-96.png"), tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(doAboutAction()));
    trayIconMenu->addAction(aboutAction);

    QAction *quitAction = new QAction(QIcon(":images/icons8-shutdown-90.png"), tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(doQuitAction()));
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon *trayIcon;
    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(libkiIcon);
    trayIcon->show();
}

void SystemTray::doQuitAction() {
    qDebug() << "SystemTray::doQuitAction";

    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":images/libki_clock.png"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Quit?"));
    msgBox.setText(tr("Are you sure you want to quit?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
    msgBox.setButtonText(QMessageBox::No, tr("No"));
    int ret = msgBox.exec();
    if( ret == QMessageBox::Yes ) {
        exit(0);
    }
}

void SystemTray::doAboutAction() {
    qDebug() << "SystemTray::doAboutAction";
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":images/libki_clock.png"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("About");
    msgBox.setText("Libki Print Manager");
    msgBox.setInformativeText("Libki.org\n©2020 Kyle M Hall, ByWater Solutions\n\n Additional icons by icons8.com");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
