#ifndef IAPETUS_CREST_H
#define IAPETUS_CREST_H

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QStringList>

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

  struct structure
  {
    int id;
    int type_id;
    int system_id;
    int alliance_id;
  };

  //list<insurance> getInsurance(int* ids, int size, string type);
  //unsigned int getMarketPrice(int itemID, int regionID, string orderType, int stationID);
  // for search in region set stationID=0
  //list<structure> getStructures(int* ids, int size);
  //string getName(int id);

private:
  //nlohmann::json getJSON(string url);
  QString getJson(QString url);
};

#endif //IAPETUS_CREST_H
