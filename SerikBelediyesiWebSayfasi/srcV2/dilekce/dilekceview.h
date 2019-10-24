#ifndef DILEKCEVIEW_H
#define DILEKCEVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "dilekcemanager.h"
#include "user.h"
#include "tcmanager.h"

class DilekceView : public ContainerWidget , public Dilekce , public DB
{
public:
    DilekceView(Dilekce* _dilekce , mongocxx::database* _db );

private:
    TCManager* mTCManager;

    void initTCView();

    void initDilekceView();

    void initCevapView();
};

#endif // DILEKCEVIEW_H
