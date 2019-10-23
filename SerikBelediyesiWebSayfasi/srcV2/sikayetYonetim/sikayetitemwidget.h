#ifndef SIKAYETITEMWIDGET_H
#define SIKAYETITEMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "sikayetitem.h"
#include "SerikBelediyesiWebSayfasi/baseItem/tcitem.h"

class newSikayetItemWidget : public ContainerWidget , public DBClass , public UserClass
{
public:
    newSikayetItemWidget(mongocxx::database* _db , UserClass &userValue );


private:
    Sikayet::SikayetItem* mCurrentSikayet;

//    void initHeader();

//    void initContent();

//    void initController();

};



class SikayetItemWidget : public ContainerWidget , public DBClass , public UserClass
{
public:
    SikayetItemWidget(mongocxx::database* _db , UserClass &userValue , const bsoncxx::oid &_oid);


private:
    Sikayet::SikayetItem* mCurrentSikayet;

    void initHeader();

    void initContent();

    void initController();

};


class AsamaItemWidget : public ContainerWidget, public Sikayet::ASAMA
{
public:
    explicit AsamaItemWidget(bsoncxx::document::view &&view);
};

#endif // SIKAYETITEMWIDGET_H
