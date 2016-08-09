#include <crest.h>

Crest::Crest(string url)  :
  m_namanager(new QNetworkAccessManager), m_url(QString::fromStdString(url))
{
}

Crest::~Crest()
{
}

string Crest::getName(int id)
{
  QString param = "inventory/types/" + QString::number(id) + "/";
  QString strReply = getJson(param);
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
  QJsonObject jsonObject = jsonResponse.object();
  return jsonObject["name"].toString().toStdString();
}

unsigned int Crest::getMarketPrice(int itemID, int regionID, string orderType, int stationID)
{

}

list<Crest::insurance> Crest::getInsurance(int *ids, int size, string type)
{

}

QString Crest::getJson(QString param)
{
  QUrl url(m_url + param);
  QNetworkReply *http = m_namanager->get(QNetworkRequest(url));
  QEventLoop eventLoop;
  BOOST_VERIFY(QObject::connect(http, SIGNAL(finished()), &eventLoop, SLOT(quit())));
  eventLoop.exec();
  if(http->error() > 0)
  {
    throw std::runtime_error(http->errorString().toStdString());
  }
  else
  {
    return http->readAll();
  }
}
