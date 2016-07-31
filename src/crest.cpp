#include <crest.h>

nlohmann::json Crest::getJSON(string url)
{
    auto response = cpr::Get(cpr::Url{url});
    if(response.status_code != 200) throw runtime_error("CREST error code " + to_string(response.status_code)
                                                             + " in " + url);
    return nlohmann::json::parse(response.text);
}

list<Crest::insurance> Crest::getInsurance(int* ids, int size, string type)
{
    list<Crest::insurance> insurances;
    auto json = Crest::getJSON("https://crest-tq.eveonline.com/insuranceprices/");
    for(auto json_item : json["items"])
    {
        for(int i=0; i<size; i++)
        {
            if(json_item["type"]["id"] == ids[i])
            {
                for(auto json_insurance : json_item["insurance"])
                {
                    if(json_insurance["level"] == type)
                    {
                        Crest::insurance in;
                        in.id = ids[i];
                        in.cost = (unsigned int)json_insurance["cost"];
                        in.payout = (unsigned int)json_insurance["payout"];
                        insurances.push_back(in);
                        break;
                    }
                }
            }
        }
    }
    return insurances;
}

unsigned int Crest::getMarketPrice(int itemID, int regionID, string orderType, int stationID)
{
    string url = "https://crest-tq.eveonline.com/market/" + to_string(regionID) + "/orders/" +
                      orderType + "/" + "?type=https://crest-tq.eveonline.com/inventory/types/" + to_string(itemID) + "/";
    auto json = Crest::getJSON(url);
    unsigned int bestPrice = 0;
    for(auto json_item : json["items"])
    {
        if(stationID == 0 || json_item["location"]["id"] == stationID)
        {
            unsigned int price = json_item["price"];
            if (bestPrice > 0) {
                if (orderType == "sell" && price < bestPrice)
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

list<Crest::structure> Crest::getStructures(int* ids, int size)
{
    list<Crest::structure> structures;
    auto json = Crest::getJSON("https://crest-tq.eveonline.com/sovereignty/structures/");
    for(auto json_item : json["items"])
    {
        for(int i=0; i<size; i++) {
            if(json_item["structureID"] == ids[i])
            {
                Crest::structure s;
                s.id = ids[i];
                s.alliance_id = (int)json_item["alliance"]["id"];
                s.system_id = (int)json_item["solarSystem"]["id"];
                s.type_id = (int)json_item["type"]["id"];
                structures.push_back(s);
            }
        }
    }
    return structures;
}

string Crest::getName(int id)
{
    string url = "https://crest-tq.eveonline.com/inventory/types/" + to_string(id) + "/";
    auto json = Crest::getJSON(url);
    return json["name"];
}
