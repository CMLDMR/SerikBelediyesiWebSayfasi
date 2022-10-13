#ifndef DBCLASS_H
#define DBCLASS_H


#include "mongoheaders.h"
#include <iostream>


using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;


class QString;




class UserClass
{
public:
    explicit UserClass(const bsoncxx::document::value &_userValue);
    UserClass( const UserClass &_user);

    bsoncxx::document::value UserValue() const;
    UserClass User() const;

    boost::optional<std::string> birim();

    boost::optional<std::string> adSoyad();

    bool yetki  (const std::string &yetkiAdi);


private:
    bsoncxx::document::value mUserValue;
};

#endif // DBCLASS_H
