#ifndef DILEKCEVIEW_H
#define DILEKCEVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "dilekcemanager.h"
#include "user.h"
#include "tcmanager.h"

class DilekceView : public ContainerWidget , public Dilekce , public DilekceManager
{
public:
    DilekceView(Dilekce* _dilekce , mongocxx::database *_db, User* _user , bool _mPublicLink = false );

private:
    TCManager* mTCManager;

    void initTCView();

    void initDilekceView();

    void initCevapView();

private:
    User* mUser;

    ContainerWidget* mAciklamaContainer;

    void addAciklama(const DilekceAciklama &aciklama);

    bool mPublicLink;
};

#endif // DILEKCEVIEW_H
