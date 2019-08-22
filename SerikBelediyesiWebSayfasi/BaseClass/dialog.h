#ifndef DIALOG_H
#define DIALOG_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

class Dialog : public WDialog , public DBClass , public UserClass
{
public:
    Dialog(mongocxx::database* _db , bsoncxx::document::value &userValue , const std::string &title = "");


};

#endif // DIALOG_H
