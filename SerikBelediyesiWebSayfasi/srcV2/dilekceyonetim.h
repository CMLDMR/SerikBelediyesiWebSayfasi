#ifndef DILEKCEYONETIM_H
#define DILEKCEYONETIM_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "dilekcemanager.h"
#include "user.h"
#include "dilekce.h"


class DilekceYonetim : public ContainerWidget , public DilekceManager , public User
{
public:
    DilekceYonetim(mongocxx::database* _db , bsoncxx::document::value _user);


    void initControlPanel();
    void initMudurPanel();

    std::unique_ptr<ContainerWidget> createButton(const std::string &buttonName , const std::string &backGroundColor, const std::string &textColor);



    void listDilekce( Dilekce &filterDilekce );

    void initDilekce( const std::string &dilekceOid );

private:

    Signal<std::string> _clickDilekceItem;


};

#endif // DILEKCEYONETIM_H
