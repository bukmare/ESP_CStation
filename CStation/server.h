#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkSession>
#include <QtNetwork>
#include <stdlib.h>
#include "abstractserver.h"
#include "clientblock.h"

class Server : public AbstractServer
{
    Q_OBJECT

public:
    Server();
    ~Server();
    void Reset();
    void StartServer();
    bool SendData(QString ip_to, QString message);
    bool SendData(QHostAddress ip_to, QString message);
    bool SendData(quint16 block_id, QString message);
    bool SendSetConfigsAndReset(QString ip_to, QString ssid, QString pssw, QString servip, quint8 stid);

    quint16 getNextBlockID(quint16 block_id);

    const QStringList getIPsList();
    ClientBlock *getClientBlock(QString ip_addr);
    ClientBlock *getClientBlock(quint32 ip_addr);
    ClientBlock *getClientBlockByID(quint16 block_id);

    int getRemotePort() const;
    void setRemotePort(int value);
    int getPort() const;
    void setPort(int value);
signals:
    void blocks_change();
    void sensors_change(quint16 block_id);
    void new_sensor(Sensor* sensor_obj);
    void new_block_ready(quint16 block_id);

private slots:
    void sessionOpened();
    void recieveConnection();
    void recieveData();
    void clientDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
    void clientBlockSensorsChange();
    void clientBlockNewSensor(Sensor* sensor_obj);
    void clientBlockReady();

private:
    int port = 0;
    int remote_port = 0;
    QMap<quint32, QTcpSocket *> *sockets;
    QMap<quint16, ClientBlock *> *clientblocks;
    QString ipAddress;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

    void processMessageGroup(QString message_gr, quint32 ipaddr);
    void processMessage(QString message, quint32 ipaddr);
};

#endif
