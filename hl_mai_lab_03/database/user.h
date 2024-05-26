#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class User{
        private:
            long _id;
            std::string _name;
            std::string _surname;
            std::string _email;
            std::string _title;
            std::string _login;
            std::string _password;

        public:

            static User fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_name() const;
            const std::string &get_surname() const;
            const std::string &get_email() const;
            const std::string &get_title() const;
            const std::string &get_login() const;
            const std::string &get_password() const;

            long&        id();
            std::string &name();
            std::string &surname();
            std::string &email();
            std::string &title();
            std::string &login();
            std::string &password();

            static void init();
            static std::optional<long> auth(std::string &login, std::string &password);
            static std::vector<User> read_all();
            static std::vector<User> search(std::string name,std::string surname, std::string login);
            void encode_password();
            void save_to_postgresql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif