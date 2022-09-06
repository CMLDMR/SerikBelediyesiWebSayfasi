#ifndef DIALOG_H
#define DIALOG_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
//#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "db.h"

class Dialog : public WDialog , public SerikBLDCore::DB , public UserClass
{
public:
    Dialog(mongocxx::database* _db , const bsoncxx::document::value &userValue , const std::string &title = "");


};

#endif // DIALOG_H
