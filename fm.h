#ifndef _PEA_FM_H_
#define _PEA_FM_H_

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

enum FmState {
    FM_PLAYING, FM_PAUSED, FM_STOPPED, FM_ERROR
};

class FM: public QObject
{
    Q_OBJECT

    public:
        FM(QObject *parent = 0): QObject(parent) {}
        ~FM() { sock.close(); }
        
        void connect(const QString &host, int port);
        void disconnect();
        bool isConnected() const;
        void sendCmd(const QString &cmd, bool dropResp = true);

        QString getRemoteAddr() const { return sock.peerName(); }
        int getRemotePort() const { return sock.peerPort(); }
        FmState getState() const { return state; }
        const QString& getError() const { return error; }
        const QString& getUser() const { return user; }
        int getChannel() const { return channel; }
        const QString& getArtist() const { return artist; }
        const QString& getTitle() const { return title; }
        const QString& getAlbum() const { return album; }
        int getYear() const { return year; };
        const QString& getCover() const { return cover; }
        const QString& getUrl() const { return url; }
        bool isLiked() const { return like; }
        int getLength() const { return len; }
        int getPosition() const { return pos; }
        
    signals:
        void fmdRespond();

    private:
        void parse(QByteArray data);

        QTcpSocket sock;

        FmState state;
        QString error;
        QString user;
        int channel;
        QString artist;
        QString title;
        QString album;
        int year;
        QString cover;
        QString url;
        bool like;
        int len;
        int pos;
};

#endif
