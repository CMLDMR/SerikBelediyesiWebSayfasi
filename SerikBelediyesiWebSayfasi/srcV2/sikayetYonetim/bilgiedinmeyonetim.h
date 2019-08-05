#ifndef BILGIEDINMEYONETIM_H
#define BILGIEDINMEYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"

class BilgiEdinmeYonetim : public ContainerWiget , public DBClass , public UserClass
{
public:
    BilgiEdinmeYonetim(mongocxx::database* _db , const bsoncxx::document::value &_userValue );
};

#endif // BILGIEDINMEYONETIM_H
