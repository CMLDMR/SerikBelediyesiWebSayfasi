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

    void initContent();


    void initController();

};


class AsamaItemWidget : public ContainerWiget, public Sikayet::ASAMA
{
public:
    explicit AsamaItemWidget(bsoncxx::document::view &&view);
};

#endif // SIKAYETITEMWIDGET_H