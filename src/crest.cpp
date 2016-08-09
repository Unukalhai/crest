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
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8(), &m_err);
  if(m_err.error != QJsonParseError::NoError) throw std::runtime_error(m_err.errorString().toStdString());
  return jsonResponse.object()["name"].toString().toStdString();
}

unsigned int Crest::getMarketPrice(int itemID, int regionID, string orderType, int stationID)
{
  QString param = "market/" + QString::number(regionID) + "/orders/" + QString::fromStdString(orderType)
                  + "/" + "?type=" + m_url + "inventory/types/" + QString::number(itemID) + "/";
  unsigned int bestPrice = 0;
  QString strReply = getJson(param);
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8(), &m_err);
  if(m_err.error != QJsonParseError::NoError) throw std::runtime_error(m_err.errorString().toStdString());
  BOOST_FOREACH(QJsonValue json_item, jsonResponse.object()["items"].toArray())
  {
    if(stationID == 0 || json_item.toObject()["location"].toObject()["id"].toInt() == stationID)
    {
      unsigned int price = json_item.toObject()["price"].toInt();
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
  list<Crest::insurance> insurances;
  QString param = "insuranceprices/";
  QString strReply = getJson(param);
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8(), &m_err);
  if(m_err.error != QJsonParseError::NoError) throw std::runtime_error(m_err.errorString().toStdString());
  BOOST_FOREACH(QJsonValue json_item, jsonResponse.object()["items"].toArray())
  {
    for(int i = 0; i < size; i++)
    {
      if(json_item.toObject()["type"].toObject()["id"].toInt() == ids[i])
      {
        BOOST_FOREACH(QJsonValue json_insurance, json_item.toObject()["insurance"].toArray())
        {
          if(json_insurance.toObject()["level"].toString().toStdString() == type)
          {
            Crest::insurance in;
            in.id = ids[i];
            in.cost = static_cast<unsigned int>(json_insurance.toObject()["cost"].toDouble());
            in.payout = static_cast<unsigned int>(json_insurance.toObject()["payout"].toDouble());
            insurances.push_back(in);
            break;
          }
        }
      }
    }
  }
  return insurances;
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
