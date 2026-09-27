#pragma once
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#include <cmath>
#include <string>
#include "curl_wrapper.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
struct Location {
	double lat;
	double lon;
};

namespace ping {
    static double haversine(double lat1, double lon1,
        double lat2, double lon2)
    {
        double dLat = (lat2 - lat1) *
            M_PI / 180.0;
        double dLon = (lon2 - lon1) *
            M_PI / 180.0;

        lat1 = (lat1)*M_PI / 180.0;
        lat2 = (lat2)*M_PI / 180.0;

        double a = pow(sin(dLat / 2), 2) +
            pow(sin(dLon / 2), 2) *
            cos(lat1) * cos(lat2);
        double rad = 6371;
        double c = 2 * asin(sqrt(a));
        return rad * c;
    }


	double estimatePing(const Location& current, const Location& target) {
        auto d = haversine(current.lat, current.lon, target.lat, target.lon);
        auto lat = target.lat;
        auto lon = target.lon;
        auto ping = -140.6791039480 + 0.1631266674 * d - 10.2758580411 * lat + 1.7775551705 * lon - 0.0000103601 * pow(d, 2) + 0.0014758493 * d * lat + 0.0002586234 * d * lon - 0.1270288494 * pow(lat, 2) - 0.0717439501 * lat * lon - 0.0116540377 * pow(lon, 2);
        return ping;
	}
}

namespace robloxIp {
    std::string parseIp(std::string serverIp) {
        auto pos = 0;
        for (auto& c : serverIp) {
            if (c == '|') {
                break;
            }
            pos++;
        }
        return serverIp.substr(0, pos);
    }
}

namespace ipGeolocation {
    std::string getUserIp() {
        auto res = curl_wrapper::http::get("https://checkip.amazonaws.com");
        res.pop_back();
        return res;
    }
    Location locateIp(std::string ip) {
        std::string url = "http://ip-api.com/json/";
        url.append(ip);
        auto response = curl_wrapper::http::get(url);
        auto obj = json::parse(response);
        Location output;
        output.lat = obj["lat"];
        output.lon = obj["lon"];
        return output;
    }
}