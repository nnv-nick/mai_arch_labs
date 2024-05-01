#include "trip.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Trip Trip::fromJSON(const std::string &str, bool from_mongo = true)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;
        Trip trip;
        std::string str_to_parse = (from_mongo ? s1+s2 : str);

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str_to_parse);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        trip.id = object->getValue<long>("id");
        trip.route_id = object->getValue<long>("route_id");

        std::string user_ids_array_str = object->getValue<std::string>("user_ids");
        Poco::Dynamic::Var user_ids_array = parser.parse(user_ids_array_str);
        Poco::JSON::Array::Ptr array = user_ids_array.extract<Poco::JSON::Array::Ptr>();

        for (auto it = array->begin(); it != array->end(); ++it) {
            trip.user_ids.push_back(it->extract<long>());
        }

        return trip;
    }

    std::optional<Trip> Trip::read_by_id(long id)
    {
        std::optional<Trip> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("trips",params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    void Trip::add()
    {
        database::Database::get().send_to_mongo("trips",toJSON());
    }

    void Trip::update()
    {
        std::map<std::string,long> params;
        params["id"] = id;       
        database::Database::get().update_mongo("trips",params,toJSON());
    }
    Poco::JSON::Object::Ptr Trip::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", id);
        root->set("route_id", route_id);

        Poco::JSON::Array::Ptr array = new Poco::JSON::Array();

        for (const auto& elem : user_ids) {
            array->add(elem);
        }
        root->set("user_ids", array);

        return root;
    }
}