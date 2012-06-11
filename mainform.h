#ifndef _PEA_MAINFORM_H_
#define _PEA_MAINFORM_H_

#include "ui_mainform.h"
#include "fm.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

class MainForm: public QMainWindow
{
    Q_OBJECT

    public:
        MainForm(QWidget *parent = 0);

    private slots:
        void updateFmStatus();
        void queryFmStatus();
        void displayCover(QNetworkReply *reply);
        void createChannelMenu(QNetworkReply *reply);

        void trayActivated(QSystemTrayIcon::ActivationReason reason);
        void connectionToggled(bool checked);
        void likeToggled(bool checked);
        void fmToggled(bool checked);
        void fmSkip();
        void fmBan();
        void channelSelected(QAction *action);

    private:
        QString presentTime(int seconds);
        void resetFmStatus();
        void setButtonEnabled(bool enabled);

        FM fm;
        QTimer timer;

        Ui::MainForm ui;
        QSystemTrayIcon tray;

        QMenu menuChannels;
        QNetworkAccessManager channelDownloader;

        QNetworkAccessManager coverDownloader;
        QString coverUrl;
};

#endif
