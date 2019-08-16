#ifndef SIKAYETITEMWIDGET_H
#define SIKAYETITEMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "sikayetitem.h"

class SikayetItemWidget : public ContainerWiget , public DBClass , public UserClass
{
public:
    SikayetItemWidget(mongocxx::database* _db , UserClass &userValue , const bsoncxx::oid &_oid);


private:
    Sikayet::SikayetItem* mCurrentSikayet;

    void initHeader();


};

#endif // SIKAYETITEMWIDGET_H
