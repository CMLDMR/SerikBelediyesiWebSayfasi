#ifndef SIKAYETYONETIMWIDGET_H
#define SIKAYETYONETIMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "sikayetitem.h"

class SikayetYonetimWidget : public ContainerWiget , public UserClass , public DBClass
{
public:
    SikayetYonetimWidget(mongocxx::database* _db , bsoncxx::document::value &userValue);
};

#endif // SIKAYETYONETIMWIDGET_H
