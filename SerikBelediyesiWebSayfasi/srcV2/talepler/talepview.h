#ifndef TALEPVIEW_H
#define TALEPVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "db.h"
#include "Talep/talep.h"
#include "Talep/talepsubitem.h"
#include "Talep/talepmanager.h"
#include "user.h"
#include "tcmanager.h"
#include "personelmanager.h"

class TalepView : public ContainerWidget , public Talep , public TalepManager
{
public:
    TalepView(Talep &talepItem ,
              mongocxx::database* _db ,
              User* _mUser,
              bool _mPublicLink = true );


private:
    User* mUser;
    bool mPublicLink;

    TCManager* mTCManager;
    PersonelManager* mPersonelManager;

    void initTCView();

    void initTalepView();

    void initTalepCevap();


    ContainerWidget* mGorevliPersonelContainer;
    void updateGorevliPersonelWidget();
    void gorevliEkle();

    void addEventItem(TalepSubItem::ItemType type_);


};

#endif // TALEPVIEW_H
