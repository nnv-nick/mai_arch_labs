#include "route.h"
#include "database.h"

#include <iostream>
#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Route Route::fromJSON(const std::string &str, bool from_mongo = true)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;

        std::string str_to_parse = (from_mongo ? s1 + s2 : str);
        Route route;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str_to_parse);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        route.id = object->getValue<long>("id");
        route.user_id = object->getValue<long>("user_id");
        route.title = object->getValue<std::string>("title");
        route.length = object->getValue<long>("length");
        route.duration = object->getValue<long>("duration");
        route.price = object->getValue<long>("price");

        std::string stops_array_str = object->getValue<std::string>("stops");
        Poco::Dynamic::Var stops_array = parser.parse(stops_array_str);
        Poco::JSON::Array::Ptr array = stops_array.extract<Poco::JSON::Array::Ptr>();

        for (auto it = array->begin(); it != array->end(); ++it) {
            Poco::JSON::Object::Ptr stop_object = it->extract<Poco::JSON::Object::Ptr>();
            route.stops.emplace_back();
            route.stops.back().country = stop_object->getValue<std::string>("country");
            route.stops.back().address = stop_object->getValue<std::string>("address");
            route.stops.back().time_on_stop = stop_object->getValue<long>("time_on_stop");
        }

        return route;
    }

    std::optional<Route> Route::read_by_id(long id)
    {
        std::optional<Route> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("routes",params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<Route> Route::read_by_user_id(long user_id)
    {
        std::vector<Route> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("routes",params);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    void Route::add()
    {
        database::Database::get().send_to_mongo("routes",toJSON());
    }

    void Route::update()
    {
        std::map<std::string,long> params;
        params["id"] = id;       
        database::Database::get().update_mongo("routes",params,toJSON());
    }
    Poco::JSON::Object::Ptr Route::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", id);
        root->set("user_id", user_id);
        root->set("title", title);
        root->set("length", length);
        root->set("duration", duration);
        root->set("price", price);

        Poco::JSON::Array::Ptr array = new Poco::JSON::Array();

        for (const auto& elem : stops) {
            Poco::JSON::Object::Ptr cur = new Poco::JSON::Object();
            cur->set("country", elem.country);
            cur->set("address", elem.address);
            cur->set("time_on_stop", elem.time_on_stop);
            array->add(cur);
        }
        root->set("stops", array);

        return root;
    }
}