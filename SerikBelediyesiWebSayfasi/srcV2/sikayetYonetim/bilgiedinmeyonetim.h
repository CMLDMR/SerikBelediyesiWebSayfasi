#ifndef BILGIEDINMEYONETIM_H
#define BILGIEDINMEYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include "bilgiedinmeitem.h"



class BilgiEdinmeYonetim : public ContainerWidget , public DBClass , public UserClass
{
public:
    BilgiEdinmeYonetim(mongocxx::database* _db , const bsoncxx::document::value &_userValue );

    void initBilgiEdinme( const bsoncxx::oid &oid );
};



class BilgiEdinmeListWidget : public ContainerWidget , public DBClass
{
public:
    explicit BilgiEdinmeListWidget(const BilgiEdinmeItem *_item);

    Signal<bsoncxx::oid> &ClickBilgiEdinme();

private:

    const BilgiEdinmeItem *item;

    Signal<bsoncxx::oid> _ClickBilgiEdinme;
};


class BilgiEdinmeWidget : public ContainerWidget , public DBClass , public UserClass
{
public:
    BilgiEdinmeWidget( mongocxx::database* _db , const bsoncxx::oid &_oid , const UserClass &user );


private:
    BilgiEdinmeItem* mItem;

    const bsoncxx::oid mOid;

    void initWidget();
};


#endif // BILGIEDINMEYONETIM_H
