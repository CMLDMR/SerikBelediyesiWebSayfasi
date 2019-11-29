#ifndef TCITEMWIDGET_H
#define TCITEMWIDGET_H

#include "SerikBelediyesiWebSayfasi/baseItem/tcitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

class TCItemWidget : public ContainerWidget , public DBClass , public UserClass
{
public:
    TCItemWidget(mongocxx::database* _db , bsoncxx::document::value &userValue);

    TCItemWidget(mongocxx::database* _db ,
                 const bsoncxx::document::value &userValue ,
                 TC::TCItem* _mTCItem );


    bsoncxx::oid oid();

private:
    TC::TCItem* mTCItem;

    bool mNewTCItem;
};

#endif // TCITEMWIDGET_H
