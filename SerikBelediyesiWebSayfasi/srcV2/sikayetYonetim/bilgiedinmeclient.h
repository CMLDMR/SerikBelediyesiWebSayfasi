#ifndef BILGIEDINMECLIENT_H
#define BILGIEDINMECLIENT_H

#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiedinmeyonetim.h"

#include "db.h"


class BilgiEdinmeClient : public SerikBLDCore::DB , public UserClass , public ContainerWidget
{
public:
    BilgiEdinmeClient(mongocxx::database* _db , const bsoncxx::document::value &_user);
};

#endif // BILGIEDINMECLIENT_H
