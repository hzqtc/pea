#include "fm.h"

#include <qjson/parser.h>

void FM::connect(const QString &host, int port)
{
    sockMutex.lock();

    sock.connectToHost(host, port);

    sockMutex.unlock();
}

void FM::disconnect()
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

void FM::sendCmd(const QString &cmd, bool dropResp)
{
    sockMutex.lock();

    if (!isConnected()) {
        state = FM_ERROR;
        error = "Fail to conncet to FMD";
        sock.close();
        sockMutex.unlock();
        return;
    }

    sock.write(cmd.toAscii().data());
    if (!sock.waitForReadyRead(1000)) {
        state = FM_ERROR;
        error = "Fail to communicate with FMD";
        sock.close();
        sockMutex.unlock();
        return;
    }
    QByteArray data = sock.readAll();

    sockMutex.unlock();

    if (!dropResp) {
        parse(data);
        emit fmdRespond();
    }
}

void FM::parse(QByteArray data)
{
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(data, &ok).toMap();

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
}
