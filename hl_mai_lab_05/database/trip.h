#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    struct Trip {
        long id;
        long route_id;
        std::vector<long> user_ids;

        static Trip fromJSON(const std::string& str, bool from_mongo);
        static std::optional<Trip> read_by_id(long id);
        void   add();
        void   update();
        Poco::JSON::Object::Ptr toJSON() const;
    };
}
