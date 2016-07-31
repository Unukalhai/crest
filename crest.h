#ifndef IAPETUS_CREST_H
#define IAPETUS_CREST_H

#include <list>
#include <exception>

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

    list<insurance> getInsurance(int* ids, int size, string type);
    unsigned int getMarketPrice(int itemID, int regionID, string orderType, int stationID);
    // for search in region set stationID=0
    list<structure> getStructures(int* ids, int size);
    string getName(int id);

private:
    nlohmann::json getJSON(string url);
};

#endif //IAPETUS_CREST_H
