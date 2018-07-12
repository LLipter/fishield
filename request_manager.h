#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>

class request_manager : public QObject
{
    Q_OBJECT
public:
    explicit request_manager(QObject *parent = nullptr);
    void send(QJsonObject json, QString location);

private:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;
    QSslConfiguration config;
    QString url;


signals:
    void get_response(QJsonObject res_json);

public slots:
    void requestFinished(QNetworkReply* reply);
};

#endif // REQUEST_MANAGER_H
