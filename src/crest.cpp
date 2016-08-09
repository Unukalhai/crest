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
  QString param = "market/" + QString::number(regionID) + "/orders/" + QString::fromStdString(orderType)
                  + "/" + "?type=" + m_url + "inventory/types/" + QString::number(itemID) + "/";
  unsigned int bestPrice = 0;
  QString strReply = getJson(param);
  QJsonParseError err;
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8(), &err);
  if(err.error != QJsonParseError::NoError) throw std::runtime_error(err.errorString().toStdString());
  QJsonObject jsonObject = jsonResponse.object();
  QJsonArray jsonArray = jsonObject["items"].toArray();
  for(int i = 0; i < jsonArray.size(); ++i)
  {
    QJsonObject json_item = jsonArray[i].toObject();
    if(stationID == 0 || json_item["location"].toObject()["id"].toInt() == stationID)
    {
      unsigned int price = json_item["price"].toInt();
      if (bestPrice > 0)
      {
        if (orderType == "sell" && price > 0 && price < bestPrice)
        {
          bestPrice = price;
        }
        else if (orderType == "buy" && price > bestPrice)
        {
          bestPrice = price;
        }
      }
      else
      {
        bestPrice = price;
      }
    }
  }
  return bestPrice;
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
