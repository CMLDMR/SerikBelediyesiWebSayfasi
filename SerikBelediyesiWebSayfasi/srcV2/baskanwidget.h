#ifndef BASKANWIDGET_H
#define BASKANWIDGET_H

//#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "db.h"

class BaskanWidget : public ContainerWidget, SerikBLDCore::DB
{
public:
    BaskanWidget(mongocxx::database* _db);
};

#endif // BASKANWIDGET_H
