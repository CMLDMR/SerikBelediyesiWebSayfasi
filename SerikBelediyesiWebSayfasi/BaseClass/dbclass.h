#ifndef DBCLASS_H
#define DBCLASS_H

#include "mongoheaders.h"


using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;


class QString;


class DBClass
{
public:
    explicit DBClass( mongocxx::database* _db );


    mongocxx::database* db();

    const std::string downloadFile(const std::string &oid , const bool &forceFilename = false);

    ///
    /// \brief uploadfile: upload file to database
    /// throw std::string if error
    /// \param filepath
    /// \return
    /// bsoncxx::types::value
    const bsoncxx::types::value uploadfile( QString filepath );

private:
    mongocxx::database* __db;
};




class UserClass
{
public:
    explicit UserClass(const bsoncxx::document::value &_userValue);


private:
    bsoncxx::document::value mUserValue;
};

#endif // DBCLASS_H
