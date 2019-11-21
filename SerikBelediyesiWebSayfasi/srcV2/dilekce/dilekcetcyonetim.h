#ifndef DILEKCETCYONETIM_H
#define DILEKCETCYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "dilekcemanager.h"
#include "dilekce.h"
#include "tc.h"
#include "dilekcelistwidgetitem.h"
#include "dilekceview.h"

class DilekceTCYonetim : public ContainerWidget , public SerikBLDCore::DilekceManager
{
public:
    DilekceTCYonetim(mongocxx::database* _db , bsoncxx::document::value *_user);


    void list();

    void listBilgiDilekce( const SerikBLDCore::Dilekce &filterDilekce );

    void initBilgiDilekce( const std::string &dilekceOid );



private:
    SerikBLDCore::TC mTCUser;

    Signal<std::string> _clickDilekceItem;
};

#endif // DILEKCETCYONETIM_H
