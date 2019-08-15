#ifndef SIKAYETYONETIMWIDGET_H
#define SIKAYETYONETIMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "sikayetitem.h"

class SikayetYonetimWidget : public ContainerWiget , public UserClass , public DBClass
{
public:
    SikayetYonetimWidget(mongocxx::database* _db , bsoncxx::document::value &userValue);


private:
    Signal<std::string> _ClickFilter;

    void initSikayetler(const std::string &durumFilter);
};

#endif // SIKAYETYONETIMWIDGET_H
