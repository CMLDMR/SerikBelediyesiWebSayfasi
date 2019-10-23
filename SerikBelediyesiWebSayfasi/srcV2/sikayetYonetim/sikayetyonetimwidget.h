#ifndef SIKAYETYONETIMWIDGET_H
#define SIKAYETYONETIMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"
#include "sikayetitem.h"

class SikayetYonetimWidget : public ContainerWidget , public UserClass , public DBClass
{
public:
    SikayetYonetimWidget(mongocxx::database* _db , bsoncxx::document::value &userValue);


private:
    Signal<std::string> _ClickFilter;

    void initSikayetler(const std::string &durumFilter);
    void initSikayetler(bsoncxx::builder::basic::document filter );
    void initSikayetlerBySahibi( const std::string &sahibi);

    void initSikayet( const bsoncxx::oid &oid );

    void createNewSikayet();

    const int limit = 20;
    int skip = 0;
    std::map<std::string,int> filterListCount;


    void Sorgula();
};

#endif // SIKAYETYONETIMWIDGET_H
