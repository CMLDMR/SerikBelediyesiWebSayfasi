#ifndef BILGIEDINMECLIENT_H
#define BILGIEDINMECLIENT_H

#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiedinmeyonetim.h"



class BilgiEdinmeClient : public DBClass , public UserClass , public ContainerWidget
{
public:
    BilgiEdinmeClient(mongocxx::database* _db , bsoncxx::document::value &_user);
};

#endif // BILGIEDINMECLIENT_H
