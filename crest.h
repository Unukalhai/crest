#ifndef IAPETUS_CREST_H
#define IAPETUS_CREST_H

#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

#include <boost/scoped_ptr.hpp>

#include <list>
#include <stdexcept>

using namespace std;

class Crest
{
public:
  struct insurance
  {
    int id = 0;
    unsigned int cost = 0;
    unsigned int payout = 0;
  };

  Crest(string url = "https://crest-tq.eveonline.com/");
  ~Crest();
  string getName(int id);
  unsigned int getMarketPrice(int itemID, int regionID, string orderType, int stationID = 0);
  list<Crest::insurance> getInsurance(int* ids, int size, string type);

private:
  QString getJson(QString param);

  boost::scoped_ptr<QNetworkAccessManager> m_namanager;
  QString m_url;
};

#endif //IAPETUS_CREST_H
