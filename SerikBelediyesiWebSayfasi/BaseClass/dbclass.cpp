#include "dbclass.h"

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <fstream>

#include <iostream>





UserClass::UserClass(const bsoncxx::document::value &_userValue)
    :mUserValue(_userValue)
{

}

UserClass::UserClass(const UserClass &_user)
    :mUserValue(_user.UserValue())
{

}

bsoncxx::document::value UserClass::UserValue() const
{
    return mUserValue;
}

UserClass UserClass::User() const
{
    return *this;
}

std::optional<std::string> UserClass::birim()
{
    try {
        return this->mUserValue.view()["Birimi"].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return std::nullopt;
    }
}

std::optional<std::string> UserClass::adSoyad()
{
    try {
        return this->mUserValue.view()["ad soyad"].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return std::nullopt;
    }
}

bool UserClass::yetki(const std::string &yetkiAdi)
{
    bool exist = false;
    try {
        auto array = this->mUserValue.view()["Yetkiler"].get_array().value;
        for( auto item : array )
        {
            if( yetkiAdi == item.get_string().value.data() )
            {
                exist = true;
                break;
            }
        }
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    return exist;
}
