#ifndef _PEA_MAINFORM_H_
#define _PEA_MAINFORM_H_

#include "ui_mainform.h"
#include "fm.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class MainForm: public QMainWindow
{
    Q_OBJECT

    public:
        MainForm(QWidget *parent = 0);

    private slots:
        void updateFmStatus();
        void queryFmStatus();
        void displayCover(QNetworkReply *reply);

        void connectionToggled(bool checked);
        void likeToggled(bool checked);
        void fmToggled(bool checked);
        void fmSkip();
        void fmBan();

    private:
        QString presentTime(int seconds);
        void resetFmStatus();
        void setButtonEnabled(bool enabled);

        Ui::MainForm ui;
        FM fm;
        QTimer timer;
        QNetworkAccessManager coverDownloader;
};

#endif
