#include "request_manager.h"
#include <QJsonParseError>
#include <QNetworkReply>

request_manager::request_manager(QObject *parent) : QObject(parent)
{
    url = "https://www.irran.top";
    reply = nullptr;

    config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2OrLater);

    request.setSslConfiguration(config);
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}


void request_manager::requestFinished(QNetworkReply* reply){
    QByteArray bytes = reply->readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << QStringLiteral("Json parsing error");
        return;
    }

    QJsonObject res_json = document.object();

    // delete reply
    reply->deleteLater();

    emit get_response(res_json);
}
