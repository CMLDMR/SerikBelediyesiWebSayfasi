#ifndef BASKANWIDGET_H
#define BASKANWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"


class BaskanWidget : public DataBaseWidget
{
public:
    BaskanWidget(mongocxx::database* _db);
};

#endif // BASKANWIDGET_H
