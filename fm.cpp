#include "fm.h"

#include <qjson/parser.h>

FM::FM(QObject *parent): QObject(parent)
{
    connect(&sock, SIGNAL(readyRead()),
            this, SLOT(readServer()));
}

FM::~FM()
{
    sock.close();
}

void FM::connectToFmd(const QString &host, int port)
{
    sockMutex.lock();
    sock.connectToHost(host, port);
    sockMutex.unlock();
}

void FM::disconnectFromFmd()
{
    sockMutex.lock();
    sock.disconnectFromHost();
    sock.close();
    sockMutex.unlock();
}

bool FM::isConnected() const
{
    return sock.state() == QAbstractSocket::ConnectedState;
}

void FM::sendCmd(const QString &cmd)
{
    if (isConnected()) {
        sockMutex.lock();
        sock.write(cmd.toAscii().data());
        sockMutex.unlock();
    }
}

void FM::readServer()
{
    QString data = QString::fromUtf8(sock.readAll().data());
    // if there are multiple response received, only process the last response
    int start = data.lastIndexOf('{');
    if (start > 0) {
        data = data.right(start);
    }
    if (parse(data.toUtf8())) {
        emit fmdRespond();
    }
}

bool FM::parse(QByteArray data)
{
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok) {
        return false;
    }

    QString st = result["status"].toString();
    if (st == "play") {
        state = FM_PLAYING;
    }
    else if (st == "pause") {
        state = FM_PAUSED;
    }
    else if (st == "stop") {
        state = FM_STOPPED;
    }
    else if (st == "error") {
        state = FM_ERROR;
    }

    switch (state) {
        case FM_ERROR:
            error = result["message"].toString();
            break;
        case FM_PLAYING:
        case FM_PAUSED:
            error = "";
            artist = result["artist"].toString();
            title = result["title"].toString();
            album = result["album"].toString();
            year = result["year"].toInt();
            cover = result["cover"].toString();
            url = "http://music.douban.com" + result["url"].toString();
            like = result["like"].toInt();
            len = result["len"].toInt();
            pos = result["pos"].toInt();
        case FM_STOPPED:
            user = result["user"].toString();
            channel = result["channel"].toInt();
            break;
    }
    return true;
}
