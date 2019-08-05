#ifndef BILGIEDINMEYONETIM_H
#define BILGIEDINMEYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include "bilgiedinmeitem.h"



class BilgiEdinmeYonetim : public ContainerWiget , public DBClass , public UserClass
{
public:
    BilgiEdinmeYonetim(mongocxx::database* _db , const bsoncxx::document::value &_userValue );
};



class BilgiEdinmeWidget : public ContainerWiget , public DBClass
{
public:
    explicit BilgiEdinmeWidget(const BilgiEdinmeItem *_item);

private:

    const BilgiEdinmeItem *item;
};


#endif // BILGIEDINMEYONETIM_H
