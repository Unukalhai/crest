#ifndef IAPETUS_CREST_H
#define IAPETUS_CREST_H

#include <cpr/cpr.h>
#include <json.hpp>

#include <list>
#include <exception>

class crest {
public:
    struct insurance{
        int id = 0;
        unsigned int cost = 0, payout = 0;
    };
    struct structure{
        int id, type_id, system_id, alliance_id;
    };

    std::list<insurance> getInsurance(int* ids, int size, std::string type);
    unsigned int getMarketPrice(int itemID, int regionID, std::string orderType, int stationID);
    // for search in region set stationID=0
    std::list<structure> getStructures(int* ids, int size);
    std::string getName(int id);
private:
    nlohmann::json getJSON(std::string url);
};

#endif //IAPETUS_CREST_H