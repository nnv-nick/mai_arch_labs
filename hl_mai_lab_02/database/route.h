#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    struct Route {
        long id;
        long user_id;
        std::string title;
        long length;
        long duration;
        long price;

        struct Stop {
            std::string country;
            std::string address;
            long time_on_stop;
        };

        std::vector<Stop> stops;

        static Route fromJSON(const std::string& str, bool from_mongo);
        static std::optional<Route> read_by_id(long id);
        static std::vector<Route> read_by_user_id(long user_id);
        void   add();
        void   update();
        Poco::JSON::Object::Ptr toJSON() const;
    };
}
