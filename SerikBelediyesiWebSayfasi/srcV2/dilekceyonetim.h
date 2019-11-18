#ifndef DILEKCEYONETIM_H
#define DILEKCEYONETIM_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "dilekcemanager.h"
#include "user.h"
#include "dilekce.h"


class DilekceYonetim : public ContainerWidget , public SerikBLDCore::DilekceManager
{
public:
    DilekceYonetim(mongocxx::database* _db , bsoncxx::document::value _user);


    void initControlPanel();
    void initMudurPanel();
    void initPersonelPanel();

    std::unique_ptr<ContainerWidget> createButton(const std::string &buttonName , const std::string &backGroundColor, const std::string &textColor);



    void listDilekce( SerikBLDCore::Dilekce &filterDilekce );

    void listBilgiDilekce( const SerikBLDCore::Dilekce &filterDilekce );

    void initDilekce( const std::string &dilekceOid );

    void initBilgiDilekce( const std::string &dilekceOid );

private:

    Signal<std::string> _clickDilekceItem;
    Signal<std::string> _clickDilekceBilgiItem;

    SerikBLDCore::User* mUser;


};

#endif // DILEKCEYONETIM_H
