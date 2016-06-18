#include <crest/crest.h>

nlohmann::json crest::getJSON(std::string url) {
    auto response = cpr::Get(cpr::Url{url});
    if(response.status_code != 200) throw std::runtime_error("CREST error code " + std::to_string(response.status_code)
                                                             + " in " + url);
    return nlohmann::json::parse(response.text);
}

std::list<crest::insurance> crest::getInsurance(int* ids, int size, std::string type) {
    std::list<crest::insurance> insurances;
    auto json = crest::getJSON("https://crest-tq.eveonline.com/insuranceprices/");
    for(auto json_item : json["items"]) {
        for(int i=0; i<size; i++) {
            if(json_item["type"]["id"] == ids[i]) {
                for(auto json_insurance : json_item["insurance"]) {
                    if(json_insurance["level"] == type) {
                        crest::insurance in;
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

unsigned int crest::getMarketPrice(int itemID, int regionID, std::string orderType, int stationID) {
    std::string url = "https://crest-tq.eveonline.com/market/" + std::to_string(regionID) + "/orders/" +
                      orderType + "/" + "?type=https://crest-tq.eveonline.com/inventory/types/" + std::to_string(itemID) + "/";
    auto json = crest::getJSON(url);
    unsigned int bestPrice = 0;
    for(auto json_item : json["items"]) {
        if(stationID == 0 || json_item["location"]["id"] == stationID) {
            unsigned int price = json_item["price"];
            if (bestPrice > 0) {
                if (orderType == "sell" && price < bestPrice) {
                    bestPrice = price;
                }
                else if (orderType == "buy" && price > bestPrice) {
                    bestPrice = price;
                }
            }
            else {
                bestPrice = price;
            }
        }
    }
    return bestPrice;
}

std::list<crest::structure> crest::getStructures(int* ids, int size) {
    std::list<crest::structure> structures;
    auto json = crest::getJSON("https://crest-tq.eveonline.com/sovereignty/structures/");
    for(auto json_item : json["items"]) {
        for(int i=0; i<size; i++) {
            if(json_item["structureID"] == ids[i]) {
                crest::structure s;
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

std::string crest::getName(int id) {
    std::string url = "https://crest-tq.eveonline.com/inventory/types/" + std::to_string(id) + "/";
    auto json = crest::getJSON(url);
    return json["name"];
}