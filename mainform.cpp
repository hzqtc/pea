#include "mainform.h"

#include <QInputDialog>

MainForm::MainForm(QWidget *parent): QMainWindow(parent)
{
    ui.setupUi(this);

    connect(&fm, SIGNAL(fmdRespond()),
            this, SLOT(updateFmStatus()));
    connect(&timer, SIGNAL(timeout()),
            this, SLOT(queryFmStatus()));
    connect(&coverDownloader, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(displayCover(QNetworkReply*)));

    connect(ui.buttonConnection, SIGNAL(clicked(bool)),
            this, SLOT(connectionToggled(bool)));
    connect(ui.buttonLike, SIGNAL(clicked(bool)),
            this, SLOT(likeToggled(bool)));
    connect(ui.buttonToggle, SIGNAL(clicked(bool)),
            this, SLOT(fmToggled(bool)));
    connect(ui.buttonSkip, SIGNAL(clicked()),
            this, SLOT(fmSkip()));
    connect(ui.buttonBan, SIGNAL(clicked()),
            this, SLOT(fmBan()));

    timer.start(1000);

    fm.connect("localhost", 10098);
    if (fm.isConnected()) {
        fm.sendCmd("info", false);
    }
    else {
        updateFmStatus();
    }
}

QString MainForm::presentTime(int seconds)
{
    int s = seconds % 60;
    int m = seconds / 60;
    return QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
}

void MainForm::resetFmStatus()
{
    ui.labelArtist->setText("");
    ui.labelAlbum->setText("");
    ui.labelTime->setText("0:00 / 0:00");
    ui.labelCover->setPixmap(NULL);
    coverUrl = "";
    ui.progressTime->setMaximum(100);
    ui.progressTime->setValue(0);
}

void MainForm::setButtonEnabled(bool enabled)
{
    ui.buttonChannels->setEnabled(enabled);
    ui.buttonLike->setEnabled(enabled);
    ui.buttonToggle->setEnabled(enabled);
    ui.buttonSkip->setEnabled(enabled);
    ui.buttonBan->setEnabled(enabled);
}

void MainForm::updateFmStatus()
{
    if (!fm.isConnected()) {
        setWindowTitle("Pea - Disconnected");
        ui.buttonConnection->setChecked(false);

        resetFmStatus();
        setButtonEnabled(false);

        return;
    }

    setWindowTitle(QString("Pea - %1:%2").arg(fm.getRemoteAddr()).arg(fm.getRemotePort()));
    ui.buttonConnection->setChecked(true);

    switch (fm.getState()) {
        case FM_ERROR:
            resetFmStatus();
            setButtonEnabled(false);
            break;
        case FM_STOPPED:
            resetFmStatus();
            setButtonEnabled(false);
            ui.buttonToggle->setEnabled(true);
            ui.buttonToggle->setChecked(true);
            break;
        case FM_PLAYING:
        case FM_PAUSED:
            ui.labelArtist->setText(QString("%1 - %2").arg(fm.getArtist(), fm.getTitle()));
            if (fm.getYear() > 0) {
                ui.labelAlbum->setText(QString("%1 (%2)").arg(fm.getAlbum()).arg(fm.getYear()));
            }
            else {
                ui.labelAlbum->setText(QString("%1").arg(fm.getAlbum()));
            }
            if (fm.getLength() > 0) {
                ui.progressTime->setMaximum(fm.getLength());
                ui.progressTime->setValue(fm.getPosition());
            }
            else {
                ui.progressTime->setMaximum(0);
                ui.progressTime->setValue(0);
            }
            ui.labelTime->setText(QString("%1 / %2").arg(presentTime(fm.getPosition())).arg(presentTime(fm.getLength())));
            if (coverUrl != fm.getCover()) {
                coverUrl = fm.getCover();
                coverDownloader.get(QNetworkRequest(QUrl(coverUrl)));
            }
            ui.buttonLike->setChecked(fm.isLiked());
            setButtonEnabled(true);
            ui.buttonToggle->setChecked(fm.getState() != FM_PLAYING);
            break;
    }
}

void MainForm::displayCover(QNetworkReply *reply)
{
    const QByteArray data = reply->readAll();

    QPixmap pixmap;
    pixmap.loadFromData(data);
    QPixmap scaledPixmap = pixmap.scaled(ui.labelCover->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.labelCover->setPixmap(scaledPixmap);

    reply->deleteLater();
}

void MainForm::queryFmStatus()
{
    if (fm.isConnected()) {
        fm.sendCmd("info", false);
    }
}

void MainForm::connectionToggled(bool checked)
{
    if (checked) {
        bool ok;
        QString server = QInputDialog::getText(this, "Connect to FMD",
                "Server (address:port):", QLineEdit::Normal, "localhost:10098", &ok);
        if (ok) {
            QStringList serverInfos = server.split(':');
            QString serverAddr = serverInfos[0];
            int serverPort = serverInfos[1].toInt();
            fm.connect(serverAddr, serverPort);
            if (fm.isConnected()) {
                fm.sendCmd("info", false);
            }
            else {
                updateFmStatus();
            }
        }
        else {
            updateFmStatus();
        }
    }
    else {
        fm.disconnect();
        updateFmStatus();
    }
}

void MainForm::likeToggled(bool checked)
{
    if (checked) {
        fm.sendCmd("rate");
    }
    else {
        fm.sendCmd("unrate");
    }
}

void MainForm::fmToggled(bool checked)
{
    if (checked) {
        fm.sendCmd("pause");
    }
    else {
        fm.sendCmd("play");
    }
}

void MainForm::fmSkip()
{
    fm.sendCmd("skip");
}

void MainForm::fmBan()
{
    fm.sendCmd("ban");
}
